#pragma once
#include<string>
#include<Windows.h>
#include<optional>
#include<stdexcept>
#include<iostream>
#include<functional>
#include<Psapi.h>
#include"wLogicException.h"
#include"wRunTimeException.h"
#include"InstructionModifier.h"

using ThreadID_t = DWORD;
using ThreadInfo_t = CREATE_THREAD_DEBUG_INFO;
using ProcessID_t = DWORD;
using PointerToBaseOfDLL_t = LPVOID;

std::optional<std::wstring> GetLastErrorMessage()noexcept;

//throws runtime error  with message as its message
void CreateRunTimeError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage = std::wstring(L"unknown error type"));

//throws logic error with message as its message
void CreateLogicError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage = std::wstring(L"unknown error type"));
void ChangeInstructionToBreakPoint(InstructionModifier& instructionModifier, InstructionModifier::InstructionAddress_t instructionAddr);
InstructionModifier::InstructionAddress_t GetThreadStartAddress(HANDLE pHandle, HANDLE tHandle);
void RevertRipAfterBreakPointException(HANDLE hThread, InstructionModifier& instructionModifier);

HANDLE GetThreadHandleByID(DWORD threadID);

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