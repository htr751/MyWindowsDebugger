# MyWindowsDebugger
A command line windows debugger written in modern c++ using Win32 API for educational pruposes.
This debugger supports the following commands:
* **stackTrace** - show for the current context the debuggee thread is in, his stack trace, at the following form:
~~~ 
> stackTrace
at main( C:\Users\htr751\Documents\C++ Projects\DebuggingAssistent\DebuggingAssistent\Source.cpp:10)
at __scrt_narrow_environment_policy::initialize_environment( d:\agent\_work\5\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79)
at _CheckForDebuggerJustMyCode( d:\agent\_work\5\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288)
at _CheckForDebuggerJustMyCode( d:\agent\_work\5\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331)
at mainCRTStartup( d:\agent\_work\5\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17)
~~~
*  **getSymbolInformation** - show info for a specific symbol in the debuggee program, for example:
~~~
> getSymbolInformation main
symbol main at address: 0x00007ff697d31900
this symbol is defined at source file C:\Users\htr751\Documents\C++ Projects\DebuggingAssistent\DebuggingAssistent\Source.cpp:8
~~~
* **getContext** - show info about the processor registers at the current state of the debuggee thread, for example:
~~~
> getContext
RAX = 0x0000000000000001
RBX = 0x0000000000000000
RCX = 0x00007ff697d4102b
RDX = 0x00000279fac97250
RSI = 0x0000000000000000
RDI = 0x00000097816ffa08
RIP = 0x00007ff697d3192a
RSP = 0x00000097816ff900
RBP = 0x00000097816ff920
R8 = 0x00000279fac9b8d0
R9 = 0x00007ffd544b1640
R10 = 0x0000000000000012
R11 = 0x00000279fac95e40
R12 = 0x0000000000000000
R13 = 0x0000000000000000
R14 = 0x0000000000000000
R15 = 0x0000000000000000
EFLAGS = 0x 0000000000000200
~~~
* **getCurrentInfo** - show information about the current state of the debuggee thread, in the following form:
~~~
symbol main at address: 0x00007ff697d31900
this symbol is defined at source file C:\Users\htr751\Documents\C++ Projects\DebuggingAssistent\DebuggingAssistent\Source.cpp:10
~~~

* **stepInto** - make the debuggee thread to continue to the next source code line. If there is a function call in the current source code line , the debuggee step into the function and stops at the first source code line of the function. for example:
assume we are currently at function main at line 10 and int that line there is a function call to function func. then after executing stepInto, if we will do showCurrentInfo we will get:
~~~
symbol func at address: 0x00007ff697d31720
this symbol is defined at source file C:\Users\htr751\Documents\C++ Projects\DebuggingAssistent\DebuggingAssistent\func.cpp:1
~~~

* **stepOver** - similiar to stepInto, but if there is a function call then the command will cuase the debuggee to run all the function and stop in the next line of the caller function.

* **stepOut** - run all the instructions of the current function and stops in the next line of the function who called the current function.
for exmaple assume we are currently at function func line 1 and we execute stepOut, then if we will execute showCurrentInfo it will print:
~~~
symbol main at address: 0x00007ff697d31900
this symbol is defined at source file C:\Users\htr751\Documents\C++ Projects\DebuggingAssistent\DebuggingAssistent\Source.cpp:11
~~~

* **breakPoint** - put a breakpoint at a specific line of a specific function. this operation requires that the debuggee program will have source line information.
* **removeBreakPoint** - removes a breakPoint that was put in a specific line of a specific function.
* **continue** - continue execution until the next break point is encoutred or the debuggee exists.
* **exit** - close the debuggee and the debugger.

# Notes and TODO's
* Currently the debugger doesn't support presentation of variables and their value and it might be supported in the future.
* Currently the debugger support only single threaded debugging. multiThreaded debuggeing might be supported in the futere.
* In addition the breakPoint command and removeBreakPoint requires that the debuggee program will have source line information (In most cases this info will be in a pdb file of the program).
