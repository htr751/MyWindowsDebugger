#include"StackWalker.h"
#include"windowsUtillities.h"
#include"SymbolInfoFactory.h"
#include"CliRendering.h"

void RetrieveCallStack(HANDLE threadHandle, HANDLE processHandle) {
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
	bool stackWalkSuccess;
	do {
		stackWalkSuccess = StackWalk64(IMAGE_FILE_MACHINE_AMD64, processHandle, threadHandle, &stackFrame,
										&threadContext, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
		if (!stackWalkSuccess)
			break;
		auto symbolInfo = symbolInfoFactory.GetSymbolInfo(processHandle, stackFrame.AddrPC.Offset);
		if (!symbolInfo.has_value())
			continue;

		IMAGEHLP_LINE64 lineInfo = { 0 };
		DWORD displacemnet;
		lineInfo.SizeOfStruct = sizeof(lineInfo);

		if (!SymGetLineFromAddr64(processHandle, stackFrame.AddrPC.Offset, &displacemnet, &lineInfo))
			continue;
		CliRendering::RenderSymbolInformation(symbolInfo.value(), lineInfo);
	} while (stackFrame.AddrReturn.Offset != 0);


}