#pragma once
#include<Windows.h>
#include"overload.h"
#include"DebugEvent.h"
#include"windowsUtillities.h"

class DebugEventController {
	DebugEvent event;
	bool continueDebug = true;
public:
	DebugEventController()noexcept = default;
	void WaitForDebugEvent();

	template<typename... Handlers>
	void ProcessDebugEvent(overload<Handlers...> handlingFunctions) const {
		this->event.HandleDebugEvent(handlingFunctions);
	}

	void ContinueDebugee(DWORD continueStatus) const noexcept;
	ThreadID_t GetCurrentThreadID() const noexcept;
	ProcessID_t GetCurrentProcessID() const noexcept;
	bool NeedToContinueDebug() const noexcept {
		return this->continueDebug;
	}
	void StopDebugging() noexcept { this->continueDebug = false; }
};