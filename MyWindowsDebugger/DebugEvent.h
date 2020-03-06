#pragma once
#include<Windows.h>
#include<variant>
#include"overload.h"

class DebugEvent {
	DWORD debugEventCode;
	DWORD processId;
	DWORD threadId;
	std::variant<EXCEPTION_DEBUG_INFO, CREATE_THREAD_DEBUG_INFO, CREATE_PROCESS_DEBUG_INFO, EXIT_THREAD_DEBUG_INFO,
		EXIT_PROCESS_DEBUG_INFO, LOAD_DLL_DEBUG_INFO, UNLOAD_DLL_DEBUG_INFO, OUTPUT_DEBUG_STRING_INFO, RIP_INFO> eventInfo;


public:
	DebugEvent() = default;
	DebugEvent(const DEBUG_EVENT& event) noexcept;
	DebugEvent& operator=(const DebugEvent& other) noexcept;
	DebugEvent& operator=(const DEBUG_EVENT& other) noexcept;

	template<typename... Handlers>
	void HandleDebugEvent(const overload<Handlers...>& handlers) const noexcept {
		std::visit(handlers, this->eventInfo);
	}

	DWORD getProcessID()const noexcept;
	DWORD getThreadID() const noexcept;

	~DebugEvent() = default;

private:
	void CreateDebugEvent(const DEBUG_EVENT& event) noexcept;
};