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

class DebuggerCore {
	mutable std::thread m_debuggerThread;

	std::variant<StackTraceTask, SymbolInforamtionTask, ContextInformationTask, SetBreakPointTask, RemoveBreakPointTask,
		ContinueTask, StepIntoTask, StepTask, ExitTask> debuggerTasks;
	mutable std::mutex conditionMutex;
	mutable std::condition_variable hasTaskVariable;
	mutable bool hasTaskCondition;

	std::queue<std::variant<OutputMessage, CreateProcessMessage, ProcessExitMessage, CreateThreadMessage,
		ThreadExitMessage, LoadDllMessage, UnLoadDllMessage, BreakPointMessage, ExceptionMessage>> debuggerMessages;
	mutable std::mutex debuggerMessagesMutex;

public:

	void StartDebugging(const std::wstring& executableName);

	template<typename Task, typename Stub = std::enable_if_t<DebuggerTaskTraits<Task>::value>>
	decltype(auto) CreateDebuggerTask(const Task& task) {
		std::unique_lock mutexGaurd{ this->conditionMutex };
		this->debuggerTasks = task;
		this->hasTaskCondition = true;
		this->hasTaskVariable.notify_one();
		mutexGaurd.unlock();

		typename Task::TaskRespone taskStatus;
		std::visit(overload{
			[&taskStatus](typename Task::TaskRespone& task) {taskStatus = task.GetTaskData(); }, 
			[](auto&& task) {}
			}, 
			this->debuggerTasks);
		return taskStatus;
	}

	bool CheckForDebuggerMessage() const;

	template<typename... MessageHandlers>
	void HandleDebuggerMessages(const overload<MessageHandlers...>& messageHandlers) {
		this->debuggerMessagesMutex.lock();
		std::queue debuggerMessages{ this->debuggerMessages };
		emptyQueue(this->debuggerMessages);
		this->debuggerMessagesMutex.unlock();

		for (auto& message : debuggerMessages)
			std::visit(messageHandlers, message);
	}

};