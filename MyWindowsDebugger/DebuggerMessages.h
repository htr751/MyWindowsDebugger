#pragma once
#include<Windows.h>
#include<string>
#include<variant>
#include"LineInfo.h"

struct OutputMessage {
	std::variant<std::string, std::wstring> message;
};

struct CreateProcessMessage {
	std::variant<std::string, std::wstring> imageName;
	LPVOID loadAddress;
	DWORD processID;
};

struct CreateThreadMessage {
	DWORD threadID;
};

struct LoadDllMessage {
	std::wstring dllName;
	LPVOID loadAddress;
};

struct UnLoadDllMessage {
	std::wstring dllName;
};

struct ThreadExitMessage {
	DWORD threadID;
	DWORD exitCode;
};

struct ProcessExitMessage {
	DWORD exitCode;
};

struct BreakPointMessage {
	LineInfo breakPointLine;
};

struct ExceptionMessage {
	LineInfo exceptionLine;
	DWORD exceptionCode;
};

