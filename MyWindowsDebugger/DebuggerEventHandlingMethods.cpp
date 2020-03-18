#include"DebuggerEventHandlingMethods.h"
#include"windowsUtillities.h"
#include<memory>
#include<Dbghelp.h>
#include<Psapi.h>
#include"FileHandle.h"
#include"CliRendering.h"
#include"PE_Parser.h"
#include"Utillities.h"
#include"StackWalker.h"
#include"Utillities.h"
#include"TaskExecuter.h"
#include"DebuggerMessages.h"
#include<sstream>

DebugEventHandlersManager::DebugEventHandlersManager(HANDLE processHandle, DebugEventController& debugEventController, DebuggerCore& debuggerCore)noexcept :m_instructionModifier(processHandle), m_debugEventController(debugEventController), debuggerCore(debuggerCore){

}

void DebugEventHandlersManager::OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo) {
	std::unique_ptr<wchar_t[]> outputString = std::make_unique<wchar_t[]>(event.nDebugStringLength);// we allocate enough space for the case of the unicode output string

	size_t numberOfBytesReadFromProcessMemory = 0;
	bool err = ReadProcessMemory(processInfo.processInfo.hProcess,outputString.get(), event.lpDebugStringData, (DWORD64)(event.nDebugStringLength) * 2, &numberOfBytesReadFromProcessMemory);

	if (!err || numberOfBytesReadFromProcessMemory != ((DWORD64)event.nDebugStringLength) * 2) 
		CreateRunTimeError(GetLastErrorMessage(), L"unknown error type");

	if (event.fUnicode) {
		std::wostringstream debuggerMessage{};
		debuggerMessage << L"debugee thread id: " << processInfo.processInfo.dwThreadId << L" inside debugee process: " << processInfo.processInfo.dwProcessId << L" says: ";
		debuggerMessage.flush();
		debuggerMessage << outputString.get() << std::endl;
		this->debuggerCore.CreateDebuggerMessage(OutputMessage{ debuggerMessage.str() });
	}
	else {
		std::ostringstream debuggerMessage{};
		debuggerMessage << "debugee thread id: " << processInfo.processInfo.dwThreadId << " inside debugee process: " << processInfo.processInfo.dwProcessId << " says: ";
		debuggerMessage.flush();
		debuggerMessage << (char*)outputString.get() << std::endl;
		this->debuggerCore.CreateDebuggerMessage(OutputMessage{ debuggerMessage.str() });
	}
}

void DebugEventHandlersManager::CreateProcessEventHandler(const CREATE_PROCESS_DEBUG_INFO& event) {
	FileHandle m_fileHandle{ event.hFile };
	this->createProcessInfo = event;

	bool symIntiallizeSuccess = SymInitialize(event.hProcess, NULL, false);
	if (!symIntiallizeSuccess)
		CreateRunTimeError(GetLastErrorMessage());

	std::size_t sizeOfImage = GetModuleSize(static_cast<HMODULE>(event.lpBaseOfImage), event.hProcess);
	_IMAGEHLP_MODULE64 moduleInfo = LoadModuleSymbols(event.hProcess, event.hFile, (PCSTR)event.lpImageName, (DWORD64)event.lpBaseOfImage, sizeOfImage);

	if (moduleInfo.SymType == SymPdb) {
		SymEnumSourceFiles(this->createProcessInfo.hProcess, (ULONG64)this->createProcessInfo.lpBaseOfImage,
			"*.CPP", &EnumSourceFilesProc, this);
		for (auto& sourceFileInfo : this->sourceFilesInfomration)
			SymEnumLines(this->createProcessInfo.hProcess, (ULONG64)this->createProcessInfo.lpBaseOfImage,
				NULL, sourceFileInfo->GetSourceFilePath().c_str(), &EnumLinesProc, &sourceFileInfo);
	}


	//setting break point at the start address of the thread
	InstructionAddress_t threadStartAddress = GetExecutableStartAddress((HMODULE)event.lpBaseOfImage, event.hProcess);
	ChangeInstructionToBreakPoint(this->m_instructionModifier, threadStartAddress);

	this->debuggerCore.CreateDebuggerMessage(CreateProcessMessage{ m_fileHandle.getFullFileName(), event.lpBaseOfImage, GetProcessId(event.hProcess) });
}

void DebugEventHandlersManager::CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event) {
	ThreadID_t threadID = GetThreadId(event.hThread);

	this->debuggerCore.CreateDebuggerMessage(CreateThreadMessage{ GetThreadId(event.hThread), (DWORD64)event.lpStartAddress });
	this->threadIdToInfoMap[threadID] = event;
}

void DebugEventHandlersManager::ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID) {
	this->debuggerCore.CreateDebuggerMessage(ThreadExitMessage{ threadID, event.dwExitCode });
}

void DebugEventHandlersManager::DllLoadDebugEventHandler(const LOAD_DLL_DEBUG_INFO& event) {
	FileHandle m_dllFileHandle{ event.hFile };
	std::wstring dllName = m_dllFileHandle.getFullFileName();

	this->baseOfDllToNameMap[event.lpBaseOfDll] = dllName;
	//HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, this->m_debugEventController.GetCurrentProcessID());

	HMODULE moduleHandle = static_cast<HMODULE>(event.lpBaseOfDll);
	std::size_t dllSize = GetModuleSize(moduleHandle, this->createProcessInfo.hProcess);
	_IMAGEHLP_MODULE64 dllInfo = LoadModuleSymbols(this->createProcessInfo.hProcess, event.hFile, static_cast<PCSTR>(wstringTostring(dllName).c_str()), (DWORD64)event.lpBaseOfDll, dllSize);
	
	if (dllInfo.SymType == SymPdb) {
		SymEnumSourceFiles(this->createProcessInfo.hProcess, (ULONG64)event.lpBaseOfDll,
			"*.CPP", &EnumSourceFilesProc, this);

		for (auto& sourceFileInfo : this->sourceFilesInfomration) {
			if(sourceFileInfo->GetSourceFileBaseAddress() == (DWORD64)event.lpBaseOfDll)
				SymEnumLines(this->createProcessInfo.hProcess, (ULONG64)event.lpBaseOfDll,
					NULL, sourceFileInfo->GetSourceFilePath().c_str(), &EnumLinesProc, &sourceFileInfo);
		}
	}

	this->debuggerCore.CreateDebuggerMessage(LoadDllMessage{ dllName, event.lpBaseOfDll });
}

void DebugEventHandlersManager::UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event) {
	if (!SymUnloadModule64(this->createProcessInfo.hProcess, (DWORD64)event.lpBaseOfDll))
		CreateRunTimeError(GetLastErrorMessage());
	this->debuggerCore.CreateDebuggerMessage(UnLoadDllMessage{ this->baseOfDllToNameMap[event.lpBaseOfDll] });
}

void DebugEventHandlersManager::ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event) {
	if (!SymUnloadModule64(this->createProcessInfo.hProcess, (DWORD64)this->createProcessInfo.lpBaseOfImage))
		CreateRunTimeError(GetLastErrorMessage());
	this->debuggerCore.CreateDebuggerMessage(ProcessExitMessage{ event.dwExitCode });
}

void DebugEventHandlersManager::ExceptionDebugEventHandler(const EXCEPTION_DEBUG_INFO& event, DWORD& continueStatus) {
	//the kernel always send break point event when creating the process
	//this variable is used to indicate whether it already has happend
	static bool firstBreakPointAlreadyHit = false;
	auto threadHandle = GetThreadHandleByID(this->m_debugEventController.GetCurrentThreadID());
	auto threadContext = GetContext(threadHandle.getHandle());

	if (event.ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT) {
		if (firstBreakPointAlreadyHit) {
			RevertRipAfterBreakPointException(threadHandle.getHandle(), this->m_instructionModifier);
			this->revertedBreakPoint = (InstructionAddress_t)threadContext.Rip; // sets the current rip to break point to examine
			this->debuggerCore.CreateDebuggerMessage(BreakPointMessage{ LineInfo{this->createProcessInfo.hProcess, (DWORD64)this->revertedBreakPoint.value()} });
			continueStatus = this->HandleSingleStepping();
		}
		else {
			firstBreakPointAlreadyHit = true;
			continueStatus = DBG_CONTINUE;
		}
	}
	else if (event.ExceptionRecord.ExceptionCode == STATUS_SINGLE_STEP) {
		this->RestoreRevertedBreakPoint();
		continueStatus = this->HandleSingleStepping();
	}
	else {
		this->debuggerCore.CreateDebuggerMessage(ExceptionMessage{ 
			LineInfo{this->createProcessInfo.hProcess, (DWORD64)threadContext.Rip}, event.ExceptionRecord.ExceptionCode });
		continueStatus = DBG_EXCEPTION_NOT_HANDLED;
	}
}

void DebugEventHandlersManager::AddSourceFile(PSOURCEFILE sourceFileInfo) {
	std::unique_ptr<SourceFileInfo> m_sourceFileInfo = std::make_unique<SourceFileInfo>(sourceFileInfo->FileName, sourceFileInfo->ModBase);
	this->sourceFilesInfomration.push_back(std::move(m_sourceFileInfo));
}

void DebugEventHandlersManager::StopDebugging() {
	TerminateProcess(this->createProcessInfo.hProcess, 0u);
	this->m_debugEventController.StopDebugging();
	this->debuggerCore.StopDebugging();
}

void DebugEventHandlersManager::RestoreRevertedBreakPoint() {
	if (this->revertedBreakPoint.has_value()) {
		auto& breakPointToExamine = this->revertedBreakPoint.value();
		for (const auto& permenantBreakPoint : this->permenantBreakPoints) {
			if (permenantBreakPoint == breakPointToExamine) {
				ChangeInstructionToBreakPoint(this->m_instructionModifier, breakPointToExamine);
				this->revertedBreakPoint.reset();
				return;
			}
		}
	}
}

DWORD DebugEventHandlersManager::HandleSingleStepping() {
	static TaskExecuter m_taskExecuter{ *this };

	if (m_taskExecuter.HasPendingTask()) {
		auto executionCode = m_taskExecuter.ContinueTaskExecution();
		if (executionCode == TaskExecuter::ExecutionCode::CONTINUE_DEBUG ||
			executionCode == TaskExecuter::ExecutionCode::TASK_COMPLETION_CONTINUE_EXECUTION)
			return DBG_CONTINUE;
	}
	// if there is no pending task or the pending task completed his execution(in success or in failiure)
	// ask the user for another task to execute

	while (!m_taskExecuter.HasPendingTask()) {
		auto task = this->debuggerCore.GetDebuggerTask();
		auto executionCode = m_taskExecuter.ExecuteTask(task);

		if (executionCode == TaskExecuter::ExecutionCode::CONTINUE_DEBUG ||
			executionCode == TaskExecuter::ExecutionCode::TASK_COMPLETION_CONTINUE_EXECUTION)
			return DBG_CONTINUE;
	}
	return DBG_CONTINUE;
}