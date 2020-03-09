#pragma once
#include<string>
#include<Windows.h>
#include<optional>
#include<stdexcept>
#include<iostream>
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