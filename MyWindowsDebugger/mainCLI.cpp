#include"CliUtillities.h"
#include"CliRendering.h"
#include"CommandParser.h"
#include"DebuggerCore.h"
#include"mainCLI.h"
#include<type_traits>

void mainCLI(DebuggerCore& debuggerCore) noexcept{
	bool continueDebug = true;
	CliRendering output{ std::wcout };

	while (continueDebug) {
		debuggerCore.HandleDebuggerMessages(overload{
			[&output, &continueDebug](auto&& message) {
				output.RenderDebuggerMessage(message);
				if (std::is_same_v<std::remove_reference_t<decltype(message)>, StopDebuggingMessage>)
					continueDebug = false;
			}
		});
		if (!continueDebug)
			break;
		std::string command;
		std::cout << ">> ";
		std::getline(std::cin, command);
		try {
			auto respone = executeCommand(debuggerCore, TokenizeCommand(command));
			std::visit(overload{
				[&output](const StackTraceData& stackTrace) {
					output.RenderStackTraceInformation(stackTrace);
				},
				[&output](const SymbolInfoFactory::SymbolInfo& symbolInfo) {
					output.RenderSymbolInformation(symbolInfo);
				},
				[&output](const CONTEXT& debuggeeContext) {
					output.RenderCpuRegisters(debuggeeContext);
				},
				[&output](const std::string& simpleMessage) {
					output.RenderSimpleString(simpleMessage);
				}
				}, respone);
		}
		catch (std::runtime_error & error) {
			output.RenderErrorMessage(error.what());
		}


	}
}