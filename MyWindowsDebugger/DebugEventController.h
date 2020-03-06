#pragma once
#include<Windows.h>
#include"overload.h"

class DebugEventController {
	DEBUG_EVENT event;
public:
	DebugEventController()noexcept;
	void WaitForDebugEvent() const noexcept;

	template<typename... Funcs>
	void ProcessDebugEvent(overload<Funcs...> handlingFunctions) const noexcept;

	void ContinueDebugee() const noexcept;
};