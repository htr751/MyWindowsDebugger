#pragma once
#include<Windows.h>
#include<unordered_map>
#include"ProcessInfo.h"
#include"windowsUtillities.h"
#include"InstructionModifier.h"


class DebugEventHandlersManager {
	std::unordered_map<ThreadID_t, ThreadInfo_t> threadIdToInfoMap;
	std::unordered_map<PointerToBaseOfDLL_t, std::wstring> baseOfDllToNameMap;
	InstructionModifier m_instructionModifier;
	CREATE_PROCESS_DEBUG_INFO debugeeProcessInfo;

public:
	DebugEventHandlersManager(HANDLE processHandle) noexcept;

	void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo);
	void CreateProcessEventHandler(const CREATE_PROCESS_DEBUG_INFO& event);
	void CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event);
	void ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID);
	void DllLoadDebugEventHandler(const LOAD_DLL_DEBUG_INFO& event);
	void UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event);
	void ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event);
	void ExceptionDebugEventHandler(const EXCEPTION_DEBUG_INFO& event, DWORD& continueStatus);
};
