#include"windowsUtillities.h"
#include<stdexcept>

std::optional<std::wstring> GetLastErrorMessage()noexcept {
	DWORD errorMessageId = GetLastError();
	if (errorMessageId == 0)
		return {};

	wchar_t* messageBuffer = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

	std::wstring message{ messageBuffer, size };
	LocalFree(messageBuffer);
	return message;
}

template<typename String>
void CreateRunTimeError(const std::optional<String>& optionalMessage, const String& alternativeMessage) {
	if (message.has_value())
		throw std::runtime_error((const char*)optionalMessage.value().c_str());
	else
		throw std::runtime_error((const char*)alternativeMessage.c_str());
}

template<typename String>
void CreateLogicError(const std::optional<String>& optionalMessage, const String& alternativeMessage) {
	if (message.has_value())
		throw std::logic_error((const char*)optionalMessage.value().c_str());
	else
		throw std::logic_error((const char*)alternativeMessage.c_str());
}