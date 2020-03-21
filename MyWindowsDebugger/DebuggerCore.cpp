#include"MyWindowsDebugger.h"
#include"DebuggerCore.h"

std::shared_ptr<DebuggerTasksContainer> DebuggerCore::GetDebuggerTask() {
	std::unique_lock  mutexGaurd{ this->conditionMutex };
	this->hasTaskVariable.wait(mutexGaurd, [this]() {return this->CheckForTask(); });
	return this->debuggerTasks;
}

bool DebuggerCore::CheckForTask() const noexcept { return this->hasTaskCondition; }

void DebuggerCore::StartDebugging(const std::wstring& executableName) {
	this->m_debuggerThread = std::thread(&DebuggerThreadEntryPoint, std::ref(*this), executableName);
	this->m_debuggerThread.detach();
}

bool DebuggerCore::CheckForDebuggerMessage() const {
	std::lock_guard mutexGaurd{ this->debuggerMessagesMutex };
	if (!this->debuggerMessages.empty())
		return false;
	return true;
}

StackTraceData DebuggerCore::GetStackTrace() {
	return this->CreateDebuggerTask(StackTraceTask());
}

SymbolInfoFactory::SymbolInfo DebuggerCore::GetSymbolInformation(const std::string& symbolName) {
	return this->CreateDebuggerTask(SymbolInforamtionTask(symbolName));
}

CONTEXT DebuggerCore::GetContext() {
	return this->CreateDebuggerTask(ContextInformationTask());
}

bool DebuggerCore::SetBreakPoint(const LineInfo& lineInfo) {
	return this->CreateDebuggerTask(SetBreakPointTask(lineInfo));
}

bool DebuggerCore::RemoveBreakPoint(const LineInfo& lineInfo) {
	return this->CreateDebuggerTask(RemoveBreakPointTask(lineInfo));
}

bool DebuggerCore::ContinueExecution() {
	return this->CreateDebuggerTask(ContinueTask());
}

bool DebuggerCore::StepInto() {
	return this->CreateDebuggerTask(StepIntoTask());
}

bool DebuggerCore::Step() {
	return this->CreateDebuggerTask(StepTask());
}

bool DebuggerCore::StopDebugging() {
	return this->CreateDebuggerTask(ExitTask());
}

bool DebuggerCore::StepOut() {
	return this->CreateDebuggerTask(StepOutTask());
}

void DebuggerCore::FinishHandleTask() noexcept {
	std::lock_guard conditonLock{ this->conditionMutex };
	this->hasTaskCondition = false;
}