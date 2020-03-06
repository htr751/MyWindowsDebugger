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