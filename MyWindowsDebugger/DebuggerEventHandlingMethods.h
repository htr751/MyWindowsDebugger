#pragma once
#include<Windows.h>
#include"ProcessInfo.h"
#include"windowsUtillities.h"
#include<unordered_map>

void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo);
void CreateProcessEventHandler(const CREATE_PROCESS_DEBUG_INFO& event);
void CreateThreadDebugEventHandler(const CREATE_THREAD_DEBUG_INFO& event, std::unordered_map<ThreadID_t, ThreadInfo_t>& threadIDtoInfoMap);
void ExitThreadDebugEventHandler(const EXIT_THREAD_DEBUG_INFO& event, ThreadID_t threadID);
void DllLoadDebugEventHandler(const LOAD_DLL_DEBUG_INFO& event, std::unordered_map<PointerToBaseOfDLL_t, std::wstring>& baseOfDLLToNameMap);
void UnLoadDllDebugEventHandler(const UNLOAD_DLL_DEBUG_INFO& event, std::unordered_map<PointerToBaseOfDLL_t, std::wstring>& baseOfDllToNameMap);
void ExitProcessDebugEventHandler(const EXIT_PROCESS_DEBUG_INFO& event);