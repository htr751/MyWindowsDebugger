#include"CliUtillities.h"

BreakPointCommandInformation::BreakPointCommandInformation(std::string sourceFileName, unsigned long lineNumber):m_sourceFileName(sourceFileName), m_lineNumber(lineNumber) {}

BreakPointCommandInformation GetBreakPointCommandInformation(const std::string& command, DebuggerCore& debuggerCore) {
	const auto lineNumberPlace = command.find(':');
	const auto functionName = (lineNumberPlace == std::string::npos) ? command : command.substr(lineNumberPlace - 1);

	const auto functionInformation = debuggerCore.GetSymbolInformation(functionName);
	auto lineNumber = (lineNumberPlace == std::string::npos) ? functionInformation.symbolSourceInfo.m_lineNumber :
		functionInformation.symbolSourceInfo.m_lineNumber + std::stoul(command.substr(lineNumberPlace + 1).c_str());

	return { functionInformation.symbolSourceInfo.m_fileName, lineNumber };
}