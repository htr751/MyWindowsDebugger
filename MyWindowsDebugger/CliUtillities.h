#pragma once
#include<string>
#include<string_view>
#include<optional>
#include"DebuggerCore.h"

struct BreakPointCommandInformation {
	std::string m_functionName;
	int m_functionLineNumber;

	BreakPointCommandInformation(std::string functionName, int functionLineNumber);
};

BreakPointCommandInformation GetBreakPointCommandInformation(const std::string& command, DebuggerCore& debuggerCore);