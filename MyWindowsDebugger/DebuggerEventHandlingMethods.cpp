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

DebugEventHandlersManager::DebugEventHandlersManager(HANDLE processHandle, const DebugEventController& debugEventController)noexcept :m_instructionModifier(processHandle), m_debugEventController(debugEventController){

}

void DebugEventHandlersManager::OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo) {
	std::unique_ptr<wchar_t[]> outputString = std::make_unique<wchar_t[]>(event.nDebugStringLength);// we allocate enough space for the case of the unicode output string

	size_t numberOfBytesReadFromProcessMemory = 0;
	bool err = ReadProcessMemory(processInfo.processInfo.hProcess,outputString.get(), event.lpDebugStringData, (DWORD64)(event.nDebugStringLength) * 2, &numberOfBytesReadFromProcessMemory);

	if (!err || numberOfBytesReadFromProcessMemory != ((DWORD64)event.nDebugStringLength) * 2)
		CreateRunTimeError(GetLastErrorMessage(), L"unknown error type");

	std::wcout << "debugee thread id: " << processInfo.processInfo.dwThreadId << " inside debugee process: " << processInfo.processInfo.dwProcessId << " says: ";
	std::wcout.flush();
	if (event.fUnicode)
		std::wcout << outputString.get() << std::endl;
	else
		std::cout << (char*)outputString.get() << std::endl;
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
}

void DebugEventHandlersManager::CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event) {
	ThreadID_t threadID = GetThreadId(event.hThread);
	std::wcout << "Thread " << event.hThread << std::dec << " (Thread ID: " << threadID << ") created at " << std::hex << event.lpStartAddress << std::endl;
	this->threadIdToInfoMap[threadID] = event;
}

void DebugEventHandlersManager::ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID) {
	std::wcout << "The thread " << std::dec << threadID << " exited with code " <<std::hex << event.dwExitCode << std::endl;
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
}

void DebugEventHandlersManager::UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event) {
	std::wcout << "dll " << this->baseOfDllToNameMap[event.lpBaseOfDll] << " unloaded" << std::endl;
	if (!SymUnloadModule64(this->createProcessInfo.hProcess, (DWORD64)event.lpBaseOfDll))
		CreateRunTimeError(GetLastErrorMessage());
}

void DebugEventHandlersManager::ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event) {
	std::wcout << "the debugee process has exited with code " << std::hex << event.dwExitCode << std::endl;
	if (!SymUnloadModule64(this->createProcessInfo.hProcess, (DWORD64)this->createProcessInfo.lpBaseOfImage))
		CreateRunTimeError(GetLastErrorMessage());
}

void DebugEventHandlersManager::ExceptionDebugEventHandler(const EXCEPTION_DEBUG_INFO& event, DWORD& continueStatus) {
	//the kernel always send break point event when creating the process
	//this variable is used to indicate whether it already has happend
	static bool firstBreakPointAlreadyHit = false;
	if (event.ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT) {
		std::wcout << "break point exception accourd at address " << event.ExceptionRecord.ExceptionAddress << std::endl;
		if (firstBreakPointAlreadyHit) {
			HANDLE threadHandle = GetThreadHandleByID(this->m_debugEventController.GetCurrentThreadID());
			RevertRipAfterBreakPointException(threadHandle, this->m_instructionModifier);
			DisplayCpuRegisters(threadHandle, &CliRendering::RenderCpuRegisters);

		}
		else {
			firstBreakPointAlreadyHit = true;
		}
		continueStatus = DBG_CONTINUE;
	}
	else {
		std::wcout << "first chance exception accourd, exception code is: " << event.ExceptionRecord.ExceptionCode << std::endl;
		std::wcout << std::endl;
		HANDLE threadHandle = GetThreadHandleByID(this->m_debugEventController.GetCurrentThreadID());
		DisplayCpuRegisters(threadHandle, &CliRendering::RenderCpuRegisters);
		std::wcout << std::endl;

		RetrieveCallStack(threadHandle, this->createProcessInfo.hProcess);

		continueStatus = DBG_EXCEPTION_NOT_HANDLED;
	}
}

void DebugEventHandlersManager::AddSourceFile(PSOURCEFILE sourceFileInfo) {
	std::unique_ptr<SourceFileInfo> m_sourceFileInfo = std::make_unique<SourceFileInfo>(sourceFileInfo->FileName, sourceFileInfo->ModBase);
	this->sourceFilesInfomration.push_back(std::move(m_sourceFileInfo));
}