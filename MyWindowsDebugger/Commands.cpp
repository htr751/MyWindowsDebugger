#include"Commands.h"

StackTraceData GetStackTrace(DebuggerCore& debuggerCore) {
	return debuggerCore.CreateDebuggerTask(StackTraceTask());
}