#include"CliUtillities.h"

BreakPointCommandInformation::BreakPointCommandInformation(std::string functionName, int functionLineNumber):m_functionName(functionName), m_functionLineNumber(functionLineNumber) {}

BreakPointCommandInformation GetBreakPointCommandInformation(const std::string& command, DebuggerCore& debuggerCore) {
	const auto lineNumberPlace = command.find(':');
	const auto functionName = (lineNumberPlace == std::string::npos) ? command : command.substr(lineNumberPlace - 1);

	const auto functionInformation = debuggerCore.GetSymbolInformation(functionName);
	auto lineNumber = (lineNumberPlace == std::string::npos) ? functionInformation.symbolSourceInfo.LineNumber :
		functionInformation.symbolSourceInfo.LineNumber + std::stoi(command.substr(lineNumberPlace + 1).c_str());

	return { functionInformation.symbolName, lineNumber };
}