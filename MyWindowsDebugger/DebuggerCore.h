#pragma once
#include<Windows.h>
#include<string>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<variant>
#include"MyWindowsDebugger.h"
#include"DebuggerTasks.h"

template<typename DebuggerView>
class DebuggerCore {
	std::thread m_debuggerThread;
	std::variant<StackTraceTask, SymbolInforamtionTask, ContextInformationTask, SetBreakPointTask, RemoveBreakPointTask,
		ContinueTask, StepIntoTask, StepTask> debuggerTasks;

	std::mutex conditionMutex;
	std::condition_variable hasTaskVariable;
	bool hasTaskCondition;

public:
	class enum ResumeMode {
		Continue,
		Step,
		StepIn,
		Exit
	};

	void StartDebugging(const std::wstring& executableName) {
		this->m_debuggerThread = std::thread(DebuggerThreadEntryPoint, *this, executableName);


	}
	void resumeDebugging(const ResumeMode& resumeMode);
	void StopDebugging();
	DebuggerView& getDebuggerView() const noexcept;
};