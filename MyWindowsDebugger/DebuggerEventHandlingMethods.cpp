#include"DebuggerEventHandlingMethods.h"
#include"windowsUtillities.h"
#include<memory>

void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo) {
	std::unique_ptr<wchar_t[]> outputString = std::make_unique<wchar_t[]>(event.nDebugStringLength);// we allocate enough space for the case of the unicode output string

	size_t numberOfBytesReadFromProcessMemory = 0;
	bool err = ReadProcessMemory(processInfo.processInfo.hProcess,outputString.get(), event.lpDebugStringData, event.nDebugStringLength * 2, &numberOfBytesReadFromProcessMemory);

	if (!err || numberOfBytesReadFromProcessMemory != event.nDebugStringLength * 2)
		CreateRunTimeError(GetLastErrorMessage(), L"unknown error type");

	std::wcout << "debugee thread id: " << processInfo.processInfo.dwThreadId << " inside debugee process: " << processInfo.processInfo.dwProcessId << " says: ";
	std::wcout.flush();
	if (event.fUnicode)
		std::wcout << outputString.get() << std::endl;
	else
		std::cout << (char*)outputString.get() << std::endl;
}