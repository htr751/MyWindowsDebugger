#pragma once
#include<string>
#include<vector>
#include"DebuggerCoreTypes.h"
#include"DebuggerCore.h"

std::vector<std::string> TokenizeCommand(const std::string& command);
DebuggerResponse executeCommand(DebuggerCore& debuggerCore, const std::vector<std::string>& command);