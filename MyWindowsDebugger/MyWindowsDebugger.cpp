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

int main(int argc, char** argv)
{
    ProcessInfo processInformation{ L"C:\\Users\\htr751\\Documents\\C++ Projects\\Exception Handling\\Debug\\Exception Handling.exe " };
    DebugEventController debugLoopEventController;
    DebugEventHandlersManager debugEventManager;

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
            debugLoopEventController.ContinueDebugee(continueStatus);
        }
        catch (const wRunTimeException& err) {
            std::wcout << err.what() << std::endl;
            return 0;
        }
    }
    return 0;
}
