#include"TaskExecuter.h"
#include"StackWalker.h"
#include"wRunTimeException.h"

TaskExecuter::TaskExecuter(DebugEventHandlersManager& debugInformation) : debugInformation(debugInformation){}
TaskExecuter::TaskState::TaskState(const LineInfo& currentLine, const SymbolInfoFactory::SymbolInfo& currentFunction) : currentLine(currentLine), currentFunction(currentFunction) {}

SourceFileInfo& SearchForSourceFile(std::vector<std::unique_ptr<SourceFileInfo>>& sourceFiles, const std::string& fileName) {
	int numberOfMatches = 0;
	SourceFileInfo* sourceFileResult = nullptr;

	for (const auto& sourceFile : sourceFiles) {
		if (sourceFile->GetSourceFilePath().find(fileName) != std::string::npos) {
			numberOfMatches++;
			sourceFileResult = sourceFile.get();
		}
	}

	if (numberOfMatches > 1)
		throw std::runtime_error("this source file name is ambigious");
	else if (numberOfMatches == 0)
		throw std::runtime_error("this source file deosn't exist");
	return *sourceFileResult;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleStackTraceTask(StackTraceTask& task) noexcept{
	auto threadHandle = GetThreadHandleByID(this->debugInformation.m_debugEventController.GetCurrentThreadID());
	task.setTaskData(RetrieveCallStack(threadHandle.getHandle(), this->debugInformation.createProcessInfo.hProcess));
	return TaskExecuter::ExecutionCode::TASK_COMPLETION;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleGetSymolInformationTask(SymbolInforamtionTask& task)noexcept {
	const auto& symbolName = task.GetSymbolName();
	const auto& symbolInformation = SymbolInfoFactory().GetSymbolInfoByName(this->debugInformation.createProcessInfo.hProcess, symbolName);
	if (!symbolInformation.has_value()) {
		task.CreateTaskException(std::runtime_error("couldn't get symbol information"));
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}
	task.setTaskData(symbolInformation.value());
	return TaskExecuter::ExecutionCode::TASK_COMPLETION;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleGetContextTask(ContextInformationTask& task)noexcept {
	auto threadHandle = GetThreadHandleByID(this->debugInformation.m_debugEventController.GetCurrentThreadID());
	try {
		const auto threadContext = GetContext(threadHandle.getHandle());
		task.setTaskData(threadContext);
		return TaskExecuter::ExecutionCode::TASK_COMPLETION;
	}
	catch (...) {
		task.CreateTaskException(std::runtime_error("internal error - couldn't get context information"));
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}
}

TaskExecuter::ExecutionCode TaskExecuter::HandleSetBreakPointTask(SetBreakPointTask& task)noexcept {
	auto breakPointInfo = task.GetInstructionInfo();
	try {
		auto& sourceFileInfo = SearchForSourceFile(this->debugInformation.sourceFilesInfomration, breakPointInfo.m_fileName);
		InstructionAddress_t breakPointAddr = (InstructionAddress_t)sourceFileInfo.GetLineInfo(breakPointInfo.m_lineNumber).m_lineAddress;
		ChangeInstructionToBreakPoint(this->debugInformation.m_instructionModifier, breakPointAddr);
		this->debugInformation.permenantBreakPoints.push_back(breakPointAddr);
		task.setTaskData(true);
		return TaskExecuter::ExecutionCode::TASK_COMPLETION;
	}
	catch (const std::runtime_error & err) {
		task.CreateTaskException(err);
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}
}

TaskExecuter::ExecutionCode TaskExecuter::HandleRemoveBreakPointTask(RemoveBreakPointTask& task)noexcept {
	auto breakPointInfo = task.GetInstructionInfo();
	try {
		auto& sourceFileInfo = SearchForSourceFile(this->debugInformation.sourceFilesInfomration, breakPointInfo.m_fileName);
		InstructionAddress_t breakPointAddr = (InstructionAddress_t)sourceFileInfo.GetLineInfo(breakPointInfo.m_lineNumber).m_lineAddress;
		this->debugInformation.m_instructionModifier.restoreInstruction(breakPointAddr);
		auto breakPoint = std::find(this->debugInformation.permenantBreakPoints.begin(),
			this->debugInformation.permenantBreakPoints.end(), breakPointAddr);
		this->debugInformation.permenantBreakPoints.erase(breakPoint);
		task.setTaskData(true);
		return TaskExecuter::ExecutionCode::TASK_COMPLETION;
	}
	catch (const std::runtime_error & err) {
		task.CreateTaskException(err);
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}
	catch (const wException & err) {
		task.CreateTaskException(std::runtime_error("break point removal failed - there wasn't break point at this place"));
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}
}

TaskExecuter::ExecutionCode TaskExecuter::HandleContinueTask(ContinueTask& task) noexcept{
	task.setTaskData(true);
	return TaskExecuter::ExecutionCode::TASK_COMPLETION_CONTINUE_EXECUTION;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleStepTask(StepTask& task) noexcept {
	auto threadHandle = GetThreadHandleByID(this->debugInformation.m_debugEventController.GetCurrentThreadID());
	CONTEXT threadContext = { 0 };
	try {
		threadContext = GetContext(threadHandle.getHandle());
	}
	catch (...) { task.CreateTaskException(std::runtime_error("couldn't complete operation")); return TaskExecuter::ExecutionCode::TASK_FAILED; }

	if (this->currentTaskState.get() == nullptr) {
		auto currentFunctionInformation = SymbolInfoFactory().GetSymbolInfo(
			this->debugInformation.createProcessInfo.hProcess, threadContext.Rip);

		if (currentFunctionInformation.has_value()) {
			task.CreateTaskException(std::runtime_error("couldn't complete operation"));
			return TaskExecuter::ExecutionCode::TASK_FAILED;
		}

		LineInfo currentLineInformation{ this->debugInformation.createProcessInfo.hProcess, threadContext.Rip };
		this->currentTaskState = std::make_unique<TaskExecuter::TaskState>(currentLineInformation, currentFunctionInformation.value());
	}
	else {
		auto currentFunctionInformation = SymbolInfoFactory().GetSymbolInfo(
			this->debugInformation.createProcessInfo.hProcess, threadContext.Rip);

		if (currentFunctionInformation.has_value()) {
			task.CreateTaskException(std::runtime_error("couldn't complete operation"));
			return TaskExecuter::ExecutionCode::TASK_FAILED;
		}

		//in case the debugee execute new function, initiate sub task of step out task
		if (this->currentTaskState->currentFunction.symbolName != currentFunctionInformation.value().symbolName) {
			this->tasksToExecute.push(std::move(this->currentTask));
			this->tasksToExecuteStates.push(std::move(this->currentTaskState));
			this->currentTask = std::make_shared<DebuggerTasksContainer>(StepOutTask());
			this->currentTaskState.reset();
			return TaskExecuter::ExecutionCode::EXECUTE_SUB_TASK;
		}

		// in case that the function hasn't changed we check if there is a line change
		LineInfo currentLineInformation{ this->debugInformation.createProcessInfo.hProcess, threadContext.Rip };
		if (this->currentTaskState->currentLine.m_lineNumber != currentLineInformation.m_lineNumber) {
			task.setTaskData(true);
			return TaskExecuter::ExecutionCode::TASK_COMPLETION;
		}
	}
	//this flags sets exeuction so execution will be stopped in the next instruction
	threadContext.EFlags |= 0x100;
	SetThreadContext(threadHandle.getHandle(), &threadContext);
	return TaskExecuter::ExecutionCode::CONTINUE_DEBUG;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleStepOutTask(StepOutTask& task) noexcept {
	auto threadHandle = GetThreadHandleByID(this->debugInformation.m_debugEventController.GetCurrentThreadID());
	CONTEXT threadContext = { 0 };
	try {
		threadContext = GetContext(threadHandle.getHandle());
	}
	catch (...) { task.CreateTaskException(std::runtime_error("couldn't complete operation")); return TaskExecuter::ExecutionCode::TASK_FAILED; }
	auto currentFunctionInformation = SymbolInfoFactory().GetSymbolInfo(
		this->debugInformation.createProcessInfo.hProcess, threadContext.Rip);
	if (!currentFunctionInformation.has_value()) {
		task.CreateTaskException(std::runtime_error("couldn't complete operation"));
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}

	LineInfo currentLineInformation{ this->debugInformation.createProcessInfo.hProcess, threadContext.Rip };

	if (this->currentTaskState.get() == nullptr) {
		this->currentTaskState = std::make_unique<TaskExecuter::TaskState>(currentLineInformation, currentFunctionInformation.value());
		
		auto currentStackFrame = RetrieveCurrentStackFrame(threadHandle.getHandle(), this->debugInformation.createProcessInfo.hProcess);
		if (!currentStackFrame.has_value()) {
			task.CreateTaskException(std::runtime_error("couldn't complete operation"));
			return TaskExecuter::ExecutionCode::TASK_FAILED;
		}
		try {
			ChangeInstructionToBreakPoint(this->debugInformation.m_instructionModifier,
				(InstructionAddress_t)currentStackFrame.value().AddrPC.Offset);
			return TaskExecuter::ExecutionCode::CONTINUE_DEBUG;
		}
		catch (...) { task.CreateTaskException(std::runtime_error("couldn't complete operation")); return TaskExecuter::ExecutionCode::TASK_FAILED; }
	}

	else {
		if (currentFunctionInformation.value().symbolName != this->currentTaskState->currentFunction.symbolName) {
			task.setTaskData(true);
			return TaskExecuter::ExecutionCode::TASK_COMPLETION;
		}
		else {
			task.CreateTaskException(std::runtime_error("couldn't complete operation"));
			return TaskExecuter::ExecutionCode::TASK_FAILED;
		}
	}
}

TaskExecuter::ExecutionCode TaskExecuter::HandleStepIntoTask(StepIntoTask& task)noexcept {
	auto threadHandle = GetThreadHandleByID(this->debugInformation.m_debugEventController.GetCurrentThreadID());
	CONTEXT threadContext = { 0 };
	try {
		threadContext = GetContext(threadHandle.getHandle());
	}
	catch (...) { task.CreateTaskException(std::runtime_error("couldn't complete operation")); return TaskExecuter::ExecutionCode::TASK_FAILED; }
	auto currentFunctionInformation = SymbolInfoFactory().GetSymbolInfo(
		this->debugInformation.createProcessInfo.hProcess, threadContext.Rip);
	if (!currentFunctionInformation.has_value()) {
		task.CreateTaskException(std::runtime_error("couldn't complete operation"));
		return TaskExecuter::ExecutionCode::TASK_FAILED;
	}

	LineInfo currentLineInformation{ this->debugInformation.createProcessInfo.hProcess, threadContext.Rip };

	if (this->currentTaskState.get() == nullptr) 
		this->currentTaskState = std::make_unique<TaskExecuter::TaskState>(currentLineInformation, currentFunctionInformation.value());
	else if (currentFunctionInformation.value().symbolName != this->currentTaskState->currentFunction.symbolName) {
		task.setTaskData(true);
		return TaskExecuter::ExecutionCode::TASK_COMPLETION;
	}

	threadContext.EFlags |= 0x100; // stop instruction execution after execution next instruction in the debuggee
	SetThreadContext(threadHandle.getHandle(), &threadContext);
	return TaskExecuter::ExecutionCode::CONTINUE_DEBUG;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleExitDebuggerTask(ExitTask& task) noexcept {
	this->debugInformation.StopDebugging();
	task.setTaskData(true);
	return TaskExecuter::ExecutionCode::TASK_COMPLETION;
}

TaskExecuter::ExecutionCode TaskExecuter::HandleTask(DebuggerTasksContainer* task) noexcept{
	TaskExecuter::ExecutionCode executionCode;
	std::visit(overload{
			[this, &executionCode](StackTraceTask& task) {
				executionCode = this->HandleStackTraceTask(task);
			},
			[this, &executionCode](SymbolInforamtionTask& task) {
				executionCode = HandleGetSymolInformationTask(task);
			},
			[this, &executionCode](ContextInformationTask& task) {
				executionCode = HandleGetContextTask(task);
			},
			[this, &executionCode](SetBreakPointTask& task) {
				executionCode = HandleSetBreakPointTask(task);
			}, 
			[this, &executionCode](RemoveBreakPointTask& task) {
				executionCode = HandleRemoveBreakPointTask(task);
			}, 
			[this, &executionCode](ContinueTask& task) {
				executionCode = HandleContinueTask(task);
			},
			[this, &executionCode](StepTask& task) {
				executionCode = HandleStepTask(task);
			}, 
			[this, &executionCode](StepOutTask& task) {
				executionCode = HandleStepOutTask(task);
			}, 
			[this, &executionCode](StepIntoTask& task) {
				executionCode = HandleStepIntoTask(task);
			}, 
			[this, &executionCode](ExitTask& task) {
				executionCode = HandleExitDebuggerTask(task);
			}
		}, *task);
	return executionCode;
}

TaskExecuter::ExecutionCode TaskExecuter::ExecuteTask(std::shared_ptr<DebuggerTasksContainer> task) {
	this->currentTask = task;
	this->currentTaskState.reset();
	while (true) {
		TaskExecuter::ExecutionCode executionCode = HandleTask(this->currentTask.get());
		if (executionCode == TaskExecuter::ExecutionCode::TASK_COMPLETION && !this->tasksToExecute.empty()) {
			this->currentTask = this->tasksToExecute.top();
			this->tasksToExecute.pop();
			this->currentTaskState = std::move(this->tasksToExecuteStates.top());
			this->tasksToExecuteStates.pop();
			continue;
		}
		else if (executionCode != TaskExecuter::ExecutionCode::EXECUTE_SUB_TASK)
			return executionCode;
	}
}