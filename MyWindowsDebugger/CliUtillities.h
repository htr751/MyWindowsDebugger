#pragma once
#include<string>
#include<string_view>
#include<optional>
#include"DebuggerCore.h"

struct BreakPointCommandInformation {
	std::string m_functionName;
	unsigned long m_functionLineNumber;

	BreakPointCommandInformation(std::string functionName, unsigned long functionLineNumber);
};

BreakPointCommandInformation GetBreakPointCommandInformation(const std::string& command, DebuggerCore& debuggerCore);