#pragma once
#include<string>
class DebuggerCore;

int DebuggerThreadEntryPoint(DebuggerCore& debuggerCore, std::wstring executableName);