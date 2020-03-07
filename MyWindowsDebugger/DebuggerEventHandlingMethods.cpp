#include"DebuggerEventHandlingMethods.h"
#include"windowsUtillities.h"
#include<memory>
#include"FileHandle.h"

void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo) {
	std::unique_ptr<wchar_t[]> outputString = std::make_unique<wchar_t[]>(event.nDebugStringLength);// we allocate enough space for the case of the unicode output string

	size_t numberOfBytesReadFromProcessMemory = 0;
	bool err = ReadProcessMemory(processInfo.processInfo.hProcess,outputString.get(), event.lpDebugStringData, event.nDebugStringLength * 2, &numberOfBytesReadFromProcessMemory);

	if (!err || numberOfBytesReadFromProcessMemory != (DWORD)event.nDebugStringLength * 2)
		CreateRunTimeError(GetLastErrorMessage(), L"unknown error type");

	std::wcout << "debugee thread id: " << processInfo.processInfo.dwThreadId << " inside debugee process: " << processInfo.processInfo.dwProcessId << " says: ";
	std::wcout.flush();
	if (event.fUnicode)
		std::wcout << outputString.get() << std::endl;
	else
		std::cout << (char*)outputString.get() << std::endl;
}

void CreateProcessEventHandler(const CREATE_PROCESS_DEBUG_INFO& event) {
	FileHandle m_fileHandle{ event.hFile };
	std::wcout << "create process event: " << std::endl;
	std::wcout << "process id: " << GetProcessId(event.hProcess) << std::endl;
	std::wcout << "thread id: " << GetThreadId(event.hThread) << std::endl;
	std::wcout << "process executable name is: " << m_fileHandle.getFullFileName() << std::endl;
}

void CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event, std::unordered_map<ThreadID_t, ThreadInfo_t>& threadIDtoInfoMap) {
	ThreadID_t threadID = GetThreadId(event.hThread);
	std::wcout << "Thread " << event.hThread << std::dec << " (Thread ID: " << threadID << ") created at " << std::hex << event.lpStartAddress << std::endl;
	threadIDtoInfoMap[threadID] = event;
}

void ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID) {
	std::wcout << "The thread " << std::dec << threadID << " exited with code " << event.dwExitCode << std::endl;
}

void DllLoadDebugEventHandler(const LOAD_DLL_DEBUG_INFO& event, std::unordered_map<PointerToBaseOfDLL_t, std::wstring>& baseOfDLLToNameMap) {
	FileHandle m_dllFileHandle{ event.hFile };
	std::wstring dllName = m_dllFileHandle.getFullFileName();

	baseOfDLLToNameMap[event.lpBaseOfDll] = dllName;
	std::wcout << "dll " << dllName << " loaded at " << event.lpBaseOfDll << std::endl;
}

void UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event, std::unordered_map<PointerToBaseOfDLL_t, std::wstring>& baseOfDllToNameMap) {
	std::wcout << "dll " << baseOfDllToNameMap[event.lpBaseOfDll] << " unloaded" << std::endl;
}

void ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event) {
	std::wcout << "the debugee process has exited with code " << std::hex << event.dwExitCode << std::endl;
}