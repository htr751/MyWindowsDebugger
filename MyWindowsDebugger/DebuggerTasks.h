#pragma once
#include<future>
#include<list>
#include<Windows.h>
#include<DbgHelp.h>
#include"SymbolInfoFactory.h"
#include"LineInfo.h"


template<typename TaskData, typename Task>
class DebuggerTask {
	std::promise<TaskData> taskData;
public:
	DebuggerTask() = default;
	TaskData GetTaskData() { return this->taskData.get_future().get(); }
	void setTaskData(const TaskData& taskData) { this->taskData.set_value(taskData); }
	Task& getFullTask() { return static_cast<Task&>(*this); }
};

struct StackTraceData {
	std::list<SymbolInfoFactory::SymbolInfo> stackTrace;
};

class StackTraceTask: public DebuggerTask<StackTraceData, StackTraceTask> {};

class SymbolInforamtionTask : public DebuggerTask<SymbolInfoFactory::SymbolInfo, SymbolInforamtionTask> {
	std::string symbolName;

public:
	SymbolInforamtionTask(const std::string& symbolName);
	SymbolInforamtionTask(std::string&& symbolName);
	const std::string& GetSymbolName() const;
};

class ContextInformationTask : DebuggerTask<CONTEXT, ContextInformationTask> {};

enum class OperationStatus { SUCCEDED, FAILED };
class SetBreakPointTask : DebuggerTask<OperationStatus, SetBreakPointTask> {
	LineInfo instructionSourceInfo;
public:
	SetBreakPointTask(const LineInfo& instructionInfo);
	const LineInfo& GetInstructionInfo() const ;
};

class RemoveBreakPointTask : DebuggerTask<OperationStatus, RemoveBreakPointTask> {
	LineInfo instructionSourceInfo;
public:
	RemoveBreakPointTask(const LineInfo& instructionInfo);
	const LineInfo& GetInstructionInfo() const;
};

class ContinueTask : DebuggerTask<OperationStatus, ContinueTask> {};
class StepIntoTask : DebuggerTask<OperationStatus, StepIntoTask> {};
class StepTask : DebuggerTask<OperationStatus, StepTask> {};