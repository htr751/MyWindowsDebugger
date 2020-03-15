#pragma once
#include<future>
#include<list>
#include<Windows.h>
#include<DbgHelp.h>
#include"SymbolInfoFactory.h"
#include"LineInfo.h"
#include"Utillities.h"


template<typename TaskData, typename Task>
class DebuggerTask {
	std::promise<TaskData> taskData;
public:
	using TaskRespone = TaskData;

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
	SymbolInforamtionTask() = delete;
	SymbolInforamtionTask(const SymbolInforamtionTask&) = default;
	SymbolInforamtionTask(SymbolInforamtionTask&&) = default;
	SymbolInforamtionTask& operator=(SymbolInforamtionTask&&) = default;
	SymbolInforamtionTask& operator=(const SymbolInforamtionTask&) = default;
	~SymbolInforamtionTask() = default;

	const std::string& GetSymbolName() const;
};

class ContextInformationTask : public DebuggerTask<CONTEXT, ContextInformationTask> {};

//The bool type for respone wont be used. instead if an error accoured in the debugger thread
//while trying to execute the task, he will inform us about it via set_excepetion on the promise of the task
class SetBreakPointTask : public DebuggerTask<bool, SetBreakPointTask> {
	LineInfo instructionSourceInfo;
public:
	SetBreakPointTask(const LineInfo& instructionInfo);
	SetBreakPointTask() = delete;
	SetBreakPointTask(const SetBreakPointTask&) = default;
	SetBreakPointTask(SetBreakPointTask&&) = default;
	SetBreakPointTask& operator=(SetBreakPointTask&&) = default;
	SetBreakPointTask& operator=(const SetBreakPointTask&) = default;
	~SetBreakPointTask() = default;

	const LineInfo& GetInstructionInfo() const ;
};

class RemoveBreakPointTask : public DebuggerTask<bool, RemoveBreakPointTask> {
	LineInfo instructionSourceInfo;
public:
	RemoveBreakPointTask(const LineInfo& instructionInfo);
	RemoveBreakPointTask() = delete;
	RemoveBreakPointTask(const RemoveBreakPointTask&) = default;
	RemoveBreakPointTask(RemoveBreakPointTask&&) = default;
	RemoveBreakPointTask& operator=(const RemoveBreakPointTask&) = default;
	RemoveBreakPointTask& operator=(RemoveBreakPointTask&&) = default;
	~RemoveBreakPointTask() = default;

	const LineInfo& GetInstructionInfo() const;
};

class ContinueTask : public DebuggerTask<bool, ContinueTask> {};
class StepIntoTask : public DebuggerTask<bool, StepIntoTask> {};
class StepTask : public DebuggerTask<bool, StepTask> {};
class ExitTask : public DebuggerTask<bool, ExitTask> {};