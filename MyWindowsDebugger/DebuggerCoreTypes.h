#pragma once
#include<variant>
#include"DebuggerTasks.h"
#include"DebuggerMessages.h"

using DebuggerTasksContainer = std::variant<StackTraceTask, SymbolInforamtionTask, ContextInformationTask, SetBreakPointTask, RemoveBreakPointTask,
	ContinueTask, StepIntoTask, StepTask, ExitTask>;

using DebuggerMessagesContainer = std::variant<OutputMessage, CreateProcessMessage, ProcessExitMessage, CreateThreadMessage,
	ThreadExitMessage, LoadDllMessage, UnLoadDllMessage, BreakPointMessage, ExceptionMessage>;

//not all tasks have real respone
using DebuggerResponse = std::variant<StackTraceTask::TaskRespone, SymbolInforamtionTask::TaskRespone, ContextInformationTask::TaskRespone, bool>;