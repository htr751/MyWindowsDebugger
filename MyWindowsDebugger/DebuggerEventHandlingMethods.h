#pragma once
#include<Windows.h>
#include<unordered_map>
#include<vector>
#include<DbgHelp.h>
#include<memory>
#include"DebugEventController.h"
#include"windowsUtillities.h"
#include"InstructionModifier.h"
#include"ProcessInfo.h"
#include"CliRendering.h"
#include"SourceFileInfo.h"
#include"DebuggerCore.h"

class DebugEventHandlersManager {
	std::unordered_map<ThreadID_t, ThreadInfo_t> threadIdToInfoMap;
	std::unordered_map<PointerToBaseOfDLL_t, std::wstring> baseOfDllToNameMap;
	InstructionModifier m_instructionModifier;
	DebugEventController& m_debugEventController;
	CREATE_PROCESS_DEBUG_INFO createProcessInfo = { 0 };
	DebuggerCore& debuggerCore;
	std::vector<std::unique_ptr<SourceFileInfo>> sourceFilesInfomration;
	std::vector<InstructionAddress_t> permenantBreakPoints;

public:
	DebugEventHandlersManager(HANDLE processHandle, DebugEventController& debugEventController, DebuggerCore& debuggerCore) noexcept;
	void AddSourceFile(PSOURCEFILE sourceFileInfo);

	void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo);
	void CreateProcessEventHandler(const CREATE_PROCESS_DEBUG_INFO& event);
	void CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event);
	void ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID);
	void DllLoadDebugEventHandler(const LOAD_DLL_DEBUG_INFO& event);
	void UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event);
	void ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event);
	void ExceptionDebugEventHandler(const EXCEPTION_DEBUG_INFO& event, DWORD& continueStatus);
	void StopDebugging();
	friend class TaskExecuter;
};
