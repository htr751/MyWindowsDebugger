#pragma once
#include<Windows.h>

class DebugEvent {
	DEBUG_EVENT event;
public:
	DebugEvent()noexcept;
	void WaitForDebugEvent() const noexcept;
	template<typename... Funcs>
	void ProcessDebugEvent() const noexcept;

};