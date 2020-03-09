#include"windowsUtillities.h"

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

void ChangeInstructionToBreakPoint(InstructionModifier& instructionModifier, InstructionModifier::InstructionAddress_t instructionAddr) {
	std::array<char, 15> changedInstruction;
	changedInstruction.fill('\xCC');
	instructionModifier.changeInstruction(instructionAddr, changedInstruction, 1);
}