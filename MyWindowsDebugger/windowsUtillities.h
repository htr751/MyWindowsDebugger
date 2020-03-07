#pragma once
#include<string>
#include<Windows.h>
#include<optional>
#include<stdexcept>
#include<iostream>
#include"wLogicException.h"
#include"wRunTimeException.h"

using ThreadID_t = DWORD;
using ThreadInfo_t = CREATE_THREAD_DEBUG_INFO;
std::optional<std::wstring> GetLastErrorMessage()noexcept;

//throws runtime error  with message as its message
void CreateRunTimeError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage);

//throws logic error with message as its message
void CreateLogicError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage);