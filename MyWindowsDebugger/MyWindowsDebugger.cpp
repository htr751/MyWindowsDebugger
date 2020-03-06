
#include <iostream>
#include<memory>
#include<Windows.h>
#include"FileHandle.h"
#include"ProcessInfo.h"
#include"DebugEventController.h"
#include"wLogicException.h"
#include"wRunTimeException.h"
#include"DebuggerEventHandlingMethods.h"

int main(int argc, char** argv)
{
    ProcessInfo processInformation{ L"C:\\Users\\htr751\\Documents\\C++ Projects\\Exception Handling\\Debug\\Exception Handling.exe " };
    DebugEventController debugLoopEventController;
    while (true) {
        try {
            debugLoopEventController.WaitForDebugEvent();
            debugLoopEventController.ProcessDebugEvent(overload{
                    [&processInformation](const OUTPUT_DEBUG_STRING_INFO& event) {OutputDebugStringEventHandler(event, processInformation); }
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
