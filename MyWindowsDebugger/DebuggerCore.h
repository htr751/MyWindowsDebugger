#pragma once
#include<Windows.h>
#include<string>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<variant>
#include"MyWindowsDebugger.h"
#include"DebuggerTasks.h"
#include<queue>
#include"DebuggerMessages.h"
#include"Utillities.h"
#include"overload.h"
#include"DebuggerTaskTraits.h"
#include<type_traits>
#include<memory>

class DebuggerCore {
	mutable std::thread m_debuggerThread;

	using DebuggerTasksContainer = std::variant<StackTraceTask, SymbolInforamtionTask, ContextInformationTask, SetBreakPointTask, RemoveBreakPointTask,
		ContinueTask, StepIntoTask, StepTask, ExitTask>;

	std::unique_ptr<DebuggerTasksContainer> debuggerTasks;
	mutable std::mutex conditionMutex;
	mutable std::condition_variable hasTaskVariable;
	mutable bool hasTaskCondition;

	std::queue<std::variant<OutputMessage, CreateProcessMessage, ProcessExitMessage, CreateThreadMessage,
		ThreadExitMessage, LoadDllMessage, UnLoadDllMessage, BreakPointMessage, ExceptionMessage>> debuggerMessages;
	mutable std::mutex debuggerMessagesMutex;

public:

	void StartDebugging(const std::wstring& executableName);
	bool CheckForDebuggerMessage() const;
	StackTraceData GetStackTrace();
	SymbolInfoFactory::SymbolInfo GetSymbolInformation(const std::string& symbolName);
	CONTEXT GetContext();
	bool SetBreakPoint(const LineInfo& lineInfo);
	bool RemoveBreakPoint(const LineInfo& lineInfo);
	bool ContinueExecution();
	bool StepInto();
	bool Step();
	bool StopDebugging();

	template<typename... MessageHandlers>
	void HandleDebuggerMessages(const overload<MessageHandlers...>& messageHandlers) {
		this->debuggerMessagesMutex.lock();
		std::queue debuggerMessages{ this->debuggerMessages };
		emptyQueue(this->debuggerMessages);
		this->debuggerMessagesMutex.unlock();

		for (auto& message : debuggerMessages)
			std::visit(messageHandlers, message);
	}

private:
	template<typename Task, typename Stub = std::enable_if_t<DebuggerTaskTraits<Task>::value>>
	decltype(auto) CreateDebuggerTask(Task&& task) {
		std::unique_lock mutexGaurd{ this->conditionMutex };
		this->debuggerTasks = std::make_unique<DebuggerTasksContainer>(std::move(task));
		this->hasTaskCondition = true;
		this->hasTaskVariable.notify_one();
		mutexGaurd.unlock();

		typename Task::TaskRespone taskRespone;
		std::visit(overload{
			[&taskRespone](Task& task) {taskRespone = task.GetTaskData(); },
			[](auto&& task) {}
			},
			*this->debuggerTasks.get());
		return taskRespone;
	}

};