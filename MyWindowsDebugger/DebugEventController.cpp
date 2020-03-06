#include"DebugEventController.h"
#include"windowsUtillities.h"
#include<stdexcept>

void DebugEventController::WaitForDebugEvent(){
	DEBUG_EVENT event = { 0 };
	bool err = WaitForDebugEventEx(&event, INFINITE);

	if (!err)
		CreateRunTimeError(GetLastErrorMessage(), std::wstring(L"unknown error type"));

	this->event = event;
}

void DebugEventController::ContinueDebugee() const noexcept {
	bool err = ContinueDebugEvent(this->event.getProcessID(), this->event.getThreadID(), DBG_CONTINUE);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage(), std::wstring(L"unknown error type"));
}