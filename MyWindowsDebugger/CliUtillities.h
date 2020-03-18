#pragma once
#include<string>
#include<string_view>
#include<optional>
#include"DebuggerCore.h"

struct BreakPointCommandInformation {
	std::string m_sourceFileName;
	unsigned long m_lineNumber;

	BreakPointCommandInformation(std::string sourceFileName, unsigned long lineNumber);
};

BreakPointCommandInformation GetBreakPointCommandInformation(const std::string& command, DebuggerCore& debuggerCore);