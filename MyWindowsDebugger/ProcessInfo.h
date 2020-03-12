#pragma once
#include<Windows.h>
#include<string>

struct ProcessInfo {
	STARTUPINFO processStartUpInfo;
	PROCESS_INFORMATION processInfo;

	ProcessInfo(const std::wstring& AbsolutePathToExe);
};

void UnitTestingForProcessInfo();