#include"CommandParser.h"
#include<sstream>
#include<algorithm>
#include<numeric>
#include<iterator>
#include"inputRange.h"
#include"CommandsExecuter.h"
#include<algorithm>
#include<cassert>

std::vector<std::string> TokenizeCommand(const std::string& command) {
	std::istringstream commandStream{ command };
	std::vector<std::string> commandTokens;
	InputRange<std::string> inputStream{ commandStream};

	for (const auto& token : inputStream)
		commandTokens.push_back(token);
	return commandTokens;
}

DebuggerResponse executeCommand(DebuggerCore& debuggerCore, const std::vector<std::string>& command) {
	if (command.empty())
		CreateRunTimeError(L"command should have at least command name");

	CommandsExecuter executer{ debuggerCore };
	const auto& commandName = command[0];
	std::vector<std::string> commandParameters{ command.size() - 1 };
	std::for_each(++command.begin(), command.end(), [&commandParameters](const auto& parameter) {commandParameters.push_back(parameter); });

	if (commandName == "breakPoint") {
		if (!executer.ExecuteSetBreakPointCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "removeBreakPoint") {
		if (!executer.ExecuteRemoveBreakPointCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "stackTrace")
		return executer.ExecuteStackTraceCommand(commandParameters);
	if (commandName == "getSymbolInformation")
		return executer.ExecuteGetSymbolInformationCommand(commandParameters);
	if (commandName == "getContext")
		return executer.ExecuteGetContextCommand(commandParameters);
	if (commandName == "continue") {
		if (!executer.ExecuteContinueCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "stepInto") {
		if (!executer.ExecuteStepIntoCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "stepOver") {
		if (!executer.ExecuteStepCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "stepOut") {
		if (!executer.ExecuteStepOutCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
	if (commandName == "exit") {
		if (!executer.ExecuteStopDebuggingCommand(commandParameters))
			CreateRunTimeError(L"unknown error accourd");
		return "operation completed successfully";
	}
}