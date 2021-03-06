#pragma once
#include<string>
#include<Windows.h>
#include<optional>
#include<stdexcept>
#include<iostream>
#include<functional>
#include<Psapi.h>
#include<DbgHelp.h>
#include"wLogicException.h"
#include"wRunTimeException.h"
#include"InstructionModifier.h"
#include"unique_handle.h"

using ThreadID_t = DWORD;
using ThreadInfo_t = CREATE_THREAD_DEBUG_INFO;
using ProcessID_t = DWORD;
using PointerToBaseOfDLL_t = LPVOID;

std::optional<std::wstring> GetLastErrorMessage()noexcept;

void ChangeInstructionToBreakPoint(InstructionModifier& instructionModifier, InstructionAddress_t instructionAddr);
InstructionAddress_t GetExecutableStartAddress(HMODULE moduleHandle, HANDLE processHandle);
void RevertRipAfterBreakPointException(HANDLE hThread, InstructionModifier& instructionModifier);

unique_handle GetThreadHandleByID(DWORD threadID);

//this function uses render func callback to render all cpu registers
template<typename RenderFunction>
void DisplayCpuRegisters(HANDLE hThread, RenderFunction&& renderFunc) {
	CONTEXT threadContext;
	threadContext.ContextFlags |= CONTEXT_ALL;

	bool err = GetThreadContext(hThread, &threadContext);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());

	std::invoke(std::forward<RenderFunction>(renderFunc), threadContext);
}

MODULEINFO GetModuleInfo(HANDLE processHandle, HMODULE module);
struct _IMAGEHLP_MODULE64 LoadModuleSymbols(HANDLE processHandle, HANDLE fileHandle, PCSTR imageName, DWORD64 baseOfImage, DWORD64 sizeOfImage);
std::size_t GetModuleSize(HMODULE moduleHandler, HANDLE processHandle);

BOOL __stdcall EnumSourceFilesProc(PSOURCEFILE sourceFileInfo, PVOID userContext);
BOOL __stdcall EnumLinesProc(PSRCCODEINFO LineInfo, PVOID UserContext);

CONTEXT GetContext(HANDLE threadHandle);

InstructionAddress_t GetExecutableMainFunctionAddress(HANDLE processHandle);