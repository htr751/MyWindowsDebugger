#include"CommandParser.h"
#include<sstream>
#include<algorithm>
#include<numeric>
#include<iterator>
#include"inputRange.h"
#include"CommandsExecuter.h"
#include<algorithm>

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

	if (commandName == "breakPoint")
		return executer.ExecuteSetBreakPointCommand(commandParameters);
	if (commandName == "removeBreakPoint")
		return executer.ExecuteRemoveBreakPointCommand(commandParameters);
	if (commandName == "stackTrace")
		return executer.ExecuteStackTraceCommand(commandParameters);
	//TODO: handle other commands after implementing debugging actions in debuggeing module
}