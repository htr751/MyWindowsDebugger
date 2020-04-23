#include"StackWalker.h"
#include"windowsUtillities.h"
#include"SymbolInfoFactory.h"
#include"CliRendering.h"

StackTraceData RetrieveCallStack(HANDLE threadHandle, HANDLE processHandle) {
	STACKFRAME64 stackFrame = { 0 };
	CONTEXT threadContext = { 0 };
	SetLastError(0);

	threadContext.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(threadHandle, &threadContext))
		CreateRunTimeError(GetLastErrorMessage());

	stackFrame.AddrPC.Offset = threadContext.Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = threadContext.Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = threadContext.Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;

	SymbolInfoFactory symbolInfoFactory;
	StackTraceData stackTraceData;
	bool stackWalkSuccess;
	do {
		stackWalkSuccess = StackWalk64(IMAGE_FILE_MACHINE_AMD64, processHandle, threadHandle, &stackFrame,
										&threadContext, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
		if (!stackWalkSuccess)
			break;
		auto symbolInfo = symbolInfoFactory.GetSymbolInfo(processHandle, stackFrame.AddrPC.Offset);
		if (!symbolInfo.has_value())
			continue;
		stackTraceData.stackTrace.push_back(symbolInfo.value());
	} while (stackFrame.AddrReturn.Offset != 0);
	return stackTraceData;

}

std::optional<STACKFRAME> RetrieveCurrentStackFrame(HANDLE threadHandle, HANDLE processHandle) {
	STACKFRAME stackFrame = { 0 };
	try {
		auto threadContext = GetContext(threadHandle);
		stackFrame.AddrPC.Offset = threadContext.Rip;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrStack.Offset = threadContext.Rsp;
		stackFrame.AddrStack.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Offset = threadContext.Rbp;
		stackFrame.AddrFrame.Mode = AddrModeFlat;

		StackWalk64(IMAGE_FILE_MACHINE_AMD64, processHandle, threadHandle, &stackFrame,
			&threadContext, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
		return stackFrame;
	}
	catch (...) {
		return {};
	}
}