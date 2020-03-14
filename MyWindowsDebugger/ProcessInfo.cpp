#include"ProcessInfo.h"
#include"windowsUtillities.h"
#include<thread>
#include<iostream>
#include<ctime>
#include"Utillities.h"

ProcessInfo::ProcessInfo(const std::wstring& AbsolutePathToExe) {
	ZeroMemory(&this->processStartUpInfo, sizeof(this->processStartUpInfo));
	this->processStartUpInfo.cb = sizeof(this->processStartUpInfo);
	ZeroMemory(&this->processInfo, sizeof(this->processInfo));

	bool result = CreateProcess(AbsolutePathToExe.c_str(), NULL, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE, NULL, NULL, &this->processStartUpInfo, &this->processInfo);
	if (!result)
		CreateLogicError(GetLastErrorMessage());
}

void UnitTestingForProcessInfo() {
	try {
		ProcessInfo pInfo{ L"C:\\Users\\htr751\\Documents\\C++ Projects\\Exception Handling\\Debug\\Exception Handling.exe" };
		std::cout << pInfo.processInfo.dwProcessId << std::endl;

		std::cout << "please press some button when you are ready to continue" << std::endl;
		std::cin.ignore();
	}
	catch (std::exception& err) {
		std::wcout << (const wchar_t*)err.what() << std::endl;
	}
}