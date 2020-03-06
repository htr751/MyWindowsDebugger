#pragma once
#include<string>
#include<Windows.h>
#include<optional>

std::optional<std::wstring> GetLastErrorMessage()noexcept;

template<typename String>
void CreateRunTimeError(const std::optional<String>& message); //throws runtimeError with message as its message

template<typename String>
void CreateLogicError(const std::optional<String>& message); //throws logicEror  with message as its message