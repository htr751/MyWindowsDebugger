#include"Utillities.h"
#include<Windows.h>
#include"wRunTimeException.h"
#include"wLogicException.h"

std::string wstringTostring(const std::wstring& str) {
	if (str.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &str[0], (int)str.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring stringTowstring(const std::string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

//throws runtime error  with message as its message
void CreateRunTimeError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage) {
	if (optionalMessage.has_value())
		throw wRunTimeException(optionalMessage.value());
	else
		throw wRunTimeException(alternativeMessage);

}

//throws logic error with message as its message
void CreateLogicError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage) {
	if (optionalMessage.has_value())
		throw wLogicException(optionalMessage.value());
	else
		throw wLogicException(alternativeMessage);
}
