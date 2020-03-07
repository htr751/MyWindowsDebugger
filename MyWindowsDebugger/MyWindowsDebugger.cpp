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
    std::unordered_map<ThreadID_t, ThreadInfo_t> threadIDtoInfoMap;
    while (true) {
        try {
            debugLoopEventController.WaitForDebugEvent();
            debugLoopEventController.ProcessDebugEvent(overload{
                    [&processInformation](const OUTPUT_DEBUG_STRING_INFO& event) {OutputDebugStringEventHandler(event, processInformation); },
                    [](const CREATE_PROCESS_DEBUG_INFO& event) {CreateProcessEventHandler(event); }, 
                    [&threadIDtoInfoMap](const CREATE_THREAD_DEBUG_INFO& event) {CreateThreadDebugEventHandler(event, threadIDtoInfoMap); },
                    [](const EXIT_THREAD_DEBUG_INFO& event) {}, 
                    [](const EXIT_PROCESS_DEBUG_INFO& event) {},
                    [](const EXCEPTION_DEBUG_INFO& event) {}, 
                    [](const LOAD_DLL_DEBUG_INFO& event) {},
                    [](const UNLOAD_DLL_DEBUG_INFO& event) {},
                    [](const RIP_INFO& event) {}
                });
            debugLoopEventController.ContinueDebugee();
        }
        catch (const wRunTimeException& err) {
            std::wcout << err.what() << std::endl;
            return 0;
        }
    }
    return 0;
}
