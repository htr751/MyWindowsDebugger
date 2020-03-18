#pragma once
#include"DebuggerTasks.h"
#include"DebuggerCore.h"
#include<string>
#include<vector>

class CommandsExecuter {
	DebuggerCore& m_debuggerCore;
public:
	CommandsExecuter(DebuggerCore& debuggerCore)noexcept;
	StackTraceData ExecuteStackTraceCommand(const std::vector<std::string>& arguments);
	SymbolInfoFactory::SymbolInfo ExecuteGetSymbolInformationCommand(const std::vector<std::string>& arguments);
	CONTEXT ExecuteGetContextCommand(const std::vector<std::string>& arguments);
	bool ExecuteSetBreakPointCommand(const std::vector<std::string>& arguments);
	bool ExecuteRemoveBreakPointCommand(const std::vector<std::string>& arguments);
	bool ExecuteContinueCommand(const std::vector<std::string>& arguments);
	bool ExecuteStepIntoCommand(const std::vector<std::string>& arguments);
	bool ExecuteStepCommand(const std::vector<std::string>& arguments);
	bool ExecuteStepOutCommand(const std::vector<std::string>& arguments);
	bool ExecuteStopDebuggingCommand(const std::vector<std::string>& arguments);
};