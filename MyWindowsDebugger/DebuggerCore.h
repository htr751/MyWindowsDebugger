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
#include"DebuggerCoreTypes.h"
#include"DebuggerMessageTraits.h"

class DebuggerCore {
	mutable std::thread m_debuggerThread;

	std::shared_ptr<DebuggerTasksContainer> debuggerTasks;
	mutable std::mutex conditionMutex;
	mutable std::condition_variable hasTaskVariable;
	mutable bool hasTaskCondition;

	std::queue<DebuggerMessagesContainer> debuggerMessages;
	mutable std::mutex debuggerMessagesMutex;

public:
	std::shared_ptr<DebuggerTasksContainer> GetDebuggerTask();
	bool CheckForTask() const noexcept;
	void FinishHandleTask() noexcept;

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
	bool StepOut();
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

	template<typename Message, typename = std::enable_if_t<DebuggerMessageTraits<Message>::value>>
	void CreateDebuggerMessage(Message&& message) {
		std::lock_guard MutexGaurd{ debuggerMessagesMutex };
		this->debuggerMessages.push(std::forward<Message>(message));
	}

private:
	template<typename Task, typename = std::enable_if_t<DebuggerTaskTraits<Task>::value>>
	decltype(auto) CreateDebuggerTask(Task&& task) {
		std::unique_lock mutexGaurd{ this->conditionMutex };
		this->debuggerTasks = std::make_shared<DebuggerTasksContainer>(std::move(task));
		this->hasTaskCondition = true;
		this->hasTaskVariable.notify_one();
		mutexGaurd.unlock();

		typename Task::TaskRespone taskRespone;
		try {
			std::visit(overload{
				[&taskRespone](Task& task) {taskRespone = task.GetTaskData(); },
				[](auto&& task) {}
				},
				*this->debuggerTasks.get());
		}
		catch (std::exception & exc) {
			//reset the task condition to notify that there is no any task
			this->FinishHandleTask();
			std::rethrow_exception(std::make_exception_ptr(exc));
		}
		//reset the task condition to notify that there is no any task
		this->FinishHandleTask();

		return taskRespone;
	}

};