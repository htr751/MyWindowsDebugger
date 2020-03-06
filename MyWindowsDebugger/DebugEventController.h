#pragma once
#include<Windows.h>
#include"overload.h"
#include"DebugEvent.h"

class DebugEventController {
	DebugEvent event;
public:
	DebugEventController()noexcept = default;
	void WaitForDebugEvent();

	template<typename... Handlers>
	void ProcessDebugEvent(overload<Handlers...> handlingFunctions) const noexcept {
		this->event.HandleDebugEvent(handlingFunctions);
	}

	void ContinueDebugee() const noexcept;
};