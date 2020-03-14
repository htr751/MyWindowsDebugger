#include"MyWindowsDebugger.h"
#include"DebuggerCore.h"

void DebuggerCore::StartDebugging(const std::wstring& executableName) {
	this->m_debuggerThread = std::thread(&DebuggerThreadEntryPoint, std::ref(*this), executableName);
}

bool DebuggerCore::CheckForDebuggerMessage() const {
	std::lock_guard mutexGaurd{ this->debuggerMessagesMutex };
	if (!this->debuggerMessages.empty())
		return false;
	return true;
}