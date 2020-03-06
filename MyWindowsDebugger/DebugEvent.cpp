#include"DebugEvent.h"

DebugEvent::DebugEvent(const DEBUG_EVENT& event)noexcept {
	this->CreateDebugEvent(event);
}

DebugEvent& DebugEvent::operator=(const DebugEvent& event)noexcept {
	this->debugEventCode = event.debugEventCode;
	this->threadId = event.threadId;
	this->processId = event.processId;
	this->eventInfo = event.eventInfo;
	return *this;
}

DebugEvent& DebugEvent::operator=(const DEBUG_EVENT& event)noexcept {
	this->CreateDebugEvent(event);
	return *this;
}

DWORD DebugEvent::getProcessID() const noexcept {
	return this->processId;
}

DWORD DebugEvent::getThreadID() const noexcept {
	return this->threadId;
}

template<typename... Handlers>
void DebugEvent::HandleDebugEvent(const overload<Handlers...>& handlers) const noexcept {
	std::visit(handlers, this->eventInfo);
}


void DebugEvent::CreateDebugEvent(const DEBUG_EVENT& event) noexcept {
	this->debugEventCode = event.dwDebugEventCode;
	this->processId = event.dwProcessId;
	this->threadId = event.dwThreadId;

	switch (event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		this->eventInfo = event.u.Exception;
		break;
	case CREATE_THREAD_DEBUG_EVENT:
		this->eventInfo = event.u.CreateThread;
		break;
	case CREATE_PROCESS_DEBUG_EVENT:
		this->eventInfo = event.u.CreateProcessInfo;
		break;
	case EXIT_THREAD_DEBUG_EVENT:
		this->eventInfo = event.u.ExitThread;
		break;
	case EXIT_PROCESS_DEBUG_EVENT:
		this->eventInfo = event.u.ExitProcess;
		break;
	case LOAD_DLL_DEBUG_EVENT:
		this->eventInfo = event.u.LoadDll;
		break;
	case UNLOAD_DLL_DEBUG_EVENT:
		this->eventInfo = event.u.UnloadDll;
		break;
	case OUTPUT_DEBUG_STRING_EVENT:
		this->eventInfo = event.u.DebugString;
		break;
	case RIP_EVENT:
		this->eventInfo = event.u.RipInfo;
		break;
	}
}