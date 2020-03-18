#include <iostream>
#include<memory>
#include<unordered_map>

#include<Windows.h>
#include"FileHandle.h"
#include"ProcessInfo.h"
#include"DebugEventController.h"
#include"wLogicException.h"
#include"wRunTimeException.h"
#include"DebuggerEventHandlingMethods.h"
#include"windowsUtillities.h"
#include"MyWindowsDebugger.h"
#include"DebuggerCore.h"

int DebuggerThreadEntryPoint(DebuggerCore& debuggerCore, std::wstring executableName) {
    ProcessInfo processInformation{ executableName };
    DebugEventController debugLoopEventController;
    DebugEventHandlersManager debugEventManager{ processInformation.processInfo.hProcess, debugLoopEventController, debuggerCore};

    bool continueDebugging = true;
    DWORD continueStatus = DBG_CONTINUE;
    while (continueDebugging) {
        try {
            debugLoopEventController.WaitForDebugEvent();
            debugLoopEventController.ProcessDebugEvent(overload{
                    [&processInformation, &debugEventManager](const OUTPUT_DEBUG_STRING_INFO& event) {debugEventManager.OutputDebugStringEventHandler(event, processInformation); },
                    [&debugEventManager](const CREATE_PROCESS_DEBUG_INFO& event) {debugEventManager.CreateProcessEventHandler(event); },
                    [&debugEventManager](const CREATE_THREAD_DEBUG_INFO& event) {debugEventManager.CreateThreadDebugEventHandler(event); },
                    [&debugLoopEventController,  &debugEventManager](const EXIT_THREAD_DEBUG_INFO& event) {debugEventManager.ExitThreadDebugEventHandler(event, debugLoopEventController.GetCurrentThreadID()); },
                    [&continueDebugging,  &debugEventManager](const EXIT_PROCESS_DEBUG_INFO& event) {debugEventManager.ExitProcessDebugEventHandler(event); continueDebugging = false; },
                    [&continueStatus,  &debugEventManager](const EXCEPTION_DEBUG_INFO& event) {debugEventManager.ExceptionDebugEventHandler(event, continueStatus); },
                    [&debugEventManager](const LOAD_DLL_DEBUG_INFO& event) {debugEventManager.DllLoadDebugEventHandler(event); },
                    [&debugEventManager](const UNLOAD_DLL_DEBUG_INFO& event) {debugEventManager.UnLoadDllDebugEventHandler(event); },
                    [&debugEventManager](const RIP_INFO& event) {}
                });
            if (!debugLoopEventController.NeedToContinueDebug())
                break;
            debugLoopEventController.ContinueDebugee(continueStatus);
        }
        catch (const wRunTimeException&) {
            debuggerCore.CreateDebuggerMessage(StopDebuggingMessage{});
            return 0;
        }
    }
    debuggerCore.CreateDebuggerMessage(StopDebuggingMessage{});
    return 0;
}
