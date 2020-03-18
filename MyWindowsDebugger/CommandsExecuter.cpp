#include"CommandsExecuter.h"
#include"CliUtillities.h"
#include"BreakPointNotFoundException.h"
CommandsExecuter::CommandsExecuter(DebuggerCore& debuggerCore) noexcept:m_debuggerCore(debuggerCore) {}

StackTraceData CommandsExecuter::ExecuteStackTraceCommand(const std::vector<std::string>& arguments) {
	if(!arguments.empty())
		CreateRunTimeError(L"stack trace command shouldn't have arguments");
	return this->m_debuggerCore.GetStackTrace();
}

SymbolInfoFactory::SymbolInfo CommandsExecuter::ExecuteGetSymbolInformationCommand(const std::vector<std::string>& arguments) {
	if (arguments.size() != 1)
		CreateRunTimeError(L"print symbol value command shouldn't have more than one argument which is the symbol name");
	return this->m_debuggerCore.GetSymbolInformation(arguments[0]);
}

CONTEXT CommandsExecuter::ExecuteGetContextCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"print context command shouldn't have arguments");
	return this->m_debuggerCore.GetContext();
}

bool CommandsExecuter::ExecuteSetBreakPointCommand(const std::vector<std::string>& arguments) {
	if (arguments.size() != 1)
		CreateRunTimeError(L"breakpoint command should have line information argument in the format FunctionName[optional]:lineNumber");
	const auto breakPointInfo = GetBreakPointCommandInformation(arguments[0], this->m_debuggerCore);
	LineInfo breakPointInformation{ breakPointInfo.m_sourceFileName, breakPointInfo.m_lineNumber};
	return this->m_debuggerCore.SetBreakPoint(breakPointInformation);
}

bool CommandsExecuter::ExecuteRemoveBreakPointCommand(const std::vector<std::string>& arguments) {
	if (arguments.size() != 1)
		CreateRunTimeError(L"remove breakpoint command should have line information argument in the format FunctionName[optional]:lineNumber");
	const auto breakPointInfo = GetBreakPointCommandInformation(arguments[0], this->m_debuggerCore);
	LineInfo breakPointInformation{ breakPointInfo.m_sourceFileName, breakPointInfo.m_lineNumber };

	return this->m_debuggerCore.RemoveBreakPoint(breakPointInformation);
}

bool CommandsExecuter::ExecuteContinueCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"continue command shouln't have arguments");
	return this->m_debuggerCore.ContinueExecution();
}

bool CommandsExecuter::ExecuteStepCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"continue command shouln't have arguments");
	return this->m_debuggerCore.Step();
}
bool CommandsExecuter::ExecuteStepIntoCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"continue command shouln't have arguments");
	return this->m_debuggerCore.StepInto();
}

bool CommandsExecuter::ExecuteStepOutCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"step out command shouldn't have arguments");
	return this->m_debuggerCore.StepOut();
}
bool CommandsExecuter::ExecuteStopDebuggingCommand(const std::vector<std::string>& arguments) {
	if (!arguments.empty())
		CreateRunTimeError(L"continue command shouln't have arguments");
	return this->m_debuggerCore.StopDebugging();
}