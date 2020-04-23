#pragma once

#include<Windows.h>
#include<DbgHelp.h>
#include"DebuggerCoreTypes.h"
#include<optional>

StackTraceData RetrieveCallStack(HANDLE threadHandle, HANDLE processHandle);
std::optional<STACKFRAME> RetrieveCurrentStackFrame(HANDLE threadHandle, HANDLE processHandle);