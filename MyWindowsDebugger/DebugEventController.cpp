#include"DebugEventController.h"
#include"windowsUtillities.h"
#include<stdexcept>

void DebugEventController::WaitForDebugEvent(){
	DEBUG_EVENT event = { 0 };
	bool err = WaitForDebugEventEx(&event, INFINITE);

	if (!err)
		CreateRunTimeError(GetLastErrorMessage());

	this->event = event;
}

void DebugEventController::ContinueDebugee(DWORD continueStatus) const noexcept {
	if (continueStatus != DBG_CONTINUE && continueStatus != DBG_EXCEPTION_NOT_HANDLED)
		CreateLogicError(L"continue status parameter is invalid");

	bool err = ContinueDebugEvent(this->event.getProcessID(), this->event.getThreadID(), continueStatus);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
}

ThreadID_t DebugEventController::GetCurrentThreadID() const noexcept {
	return this->event.getThreadID();
}

ProcessID_t DebugEventController::GetCurrentProcessID() const noexcept {
	return this->event.getProcessID();
}