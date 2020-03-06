#include"windowsUtillities.h"

std::optional<std::wstring> GetLastErrorMessage() {
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