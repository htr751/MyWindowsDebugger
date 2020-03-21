#include<thread>
#include"MyWindowsDebugger.h"
#include<iostream>
#include"DebuggerTaskTraits.h"
#include"CommandParser.h"
#include<variant>
#include"overload.h"
#include"mainCLI.h"

int main(int argc, char* argv[]) {
	//if (argc != 2) {
	//	std::wcout << L"error - too few arguments to the program" << std::endl;
	//	return 0;
	//}
	//std::wstring executableName{ stringTowstring(argv[1]) };
	std::wstring executableName{ L"C:\\Users\\htr751\\Documents\\C++ Projects\\DebuggingAssistent\\x64\\Debug\\DebuggingAssistent.exe" };
	DebuggerCore debuggerCore{};
	//DebuggerThreadEntryPoint(debuggerCore, executableName);
	debuggerCore.StartDebugging(executableName);
	mainCLI(debuggerCore);
	return 0;
}