#pragma once
#include<string>
#include<Windows.h>
#include<optional>

std::optional<std::wstring> GetLastErrorMessage()noexcept;