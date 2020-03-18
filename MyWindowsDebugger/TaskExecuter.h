#pragma once
#include"DebuggerCoreTypes.h"
#include"SymbolInfoFactory.h"
#include"DebuggerEventHandlingMethods.h"
#include<stack>

//this class acts as delegated class of DebugEventHandlersManager that responsible to execute tasks
class TaskExecuter {
public:
	enum class ExecutionCode{CONTINUE_DEBUG, TASK_COMPLETION,
		TASK_FAILED, TASK_COMPLETION_CONTINUE_EXECUTION, EXECUTE_SUB_TASK};
private:
	struct TaskState {
		//this is not the line info contained whitin the currentFunction SymbolInfo
		//his line info is the line info of the function and not the line info of the current line the debuggee is in
		LineInfo currentLine;
		SymbolInfoFactory::SymbolInfo currentFunction;

		TaskState(const LineInfo& currentLine, const SymbolInfoFactory::SymbolInfo& currentFunction);
		~TaskState() = default;
	};

	std::shared_ptr<DebuggerTasksContainer> currentTask;//holds the currently running task
	std::unique_ptr<TaskState> currentTaskState;

	//this vector contains tasks that may be intitated by currentTask and need to be 
	//completed before currentTask can continue execution
	std::stack<std::shared_ptr<DebuggerTasksContainer>> tasksToExecute;
	std::stack<std::unique_ptr<TaskState>> tasksToExecuteStates;//includes states for all subtasks that happen concurrently

	DebugEventHandlersManager& debugInformation;
public:
	TaskExecuter(DebugEventHandlersManager& debugInformation);
	ExecutionCode ExecuteTask(std::shared_ptr<DebuggerTasksContainer> task);
	ExecutionCode ContinueTaskExecution() { return ExecuteTask(this->currentTask); }
	bool HasPendingTask() const noexcept { return this->currentTask.get() != nullptr; }
private:
	ExecutionCode HandleTask(DebuggerTasksContainer* task) noexcept;
	ExecutionCode HandleStackTraceTask(StackTraceTask& task) noexcept;
	ExecutionCode HandleGetSymolInformationTask(SymbolInforamtionTask& task) noexcept;
	ExecutionCode HandleGetContextTask(ContextInformationTask& task)noexcept;
	ExecutionCode HandleSetBreakPointTask(SetBreakPointTask& task)noexcept;
	ExecutionCode HandleRemoveBreakPointTask(RemoveBreakPointTask& task)noexcept;
	ExecutionCode HandleContinueTask(ContinueTask& task) noexcept;
	ExecutionCode HandleStepTask(StepTask& task) noexcept;
	ExecutionCode HandleStepOutTask(StepOutTask& task) noexcept;
	ExecutionCode HandleStepIntoTask(StepIntoTask& task) noexcept;
	ExecutionCode HandleExitDebuggerTask(ExitTask& task) noexcept;
};