#include"windowsUtillities.h"
#include<DbgHelp.h>
#include<memory>
#include"PE_Parser.h"

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

void RevertRipAfterBreakPointException(HANDLE hThread, InstructionModifier& instructionModifier) {
	CONTEXT threadCurrentContext;
	threadCurrentContext.ContextFlags |= CONTEXT_ALL;
	bool err = GetThreadContext(hThread, &threadCurrentContext);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());

	//the processor executed break point instruction which is one byte length so it advanced rip by 1
	//get rip to be with its original value
	threadCurrentContext.Rip--;
	err = SetThreadContext(hThread, &threadCurrentContext);
	if (!err)
		CreateRunTimeError(GetLastErrorMessage());
	try {
		instructionModifier.restoreInstruction((InstructionModifier::InstructionAddress_t)threadCurrentContext.Rip);
	}
	catch (const wLogicException & err) { std::wcout << err.what() << std::endl; throw; }

}

HANDLE GetThreadHandleByID(DWORD threadID) {
	HANDLE tHandle = OpenThread(THREAD_ALL_ACCESS, false, threadID);
	if (!tHandle)
		CreateRunTimeError(GetLastErrorMessage());
	return tHandle;
}

MODULEINFO GetModuleInfo(HANDLE processHandle, HMODULE module) {
	MODULEINFO info = { 0 };
	if (!GetModuleInformation(processHandle, module, &info, sizeof(info)))
		CreateRunTimeError(GetLastErrorMessage());

	return info;
}

_IMAGEHLP_MODULE64 LoadModuleSymbols(HANDLE processHandle, HANDLE fileHandle, PCSTR imageName, DWORD64 baseOfImage, DWORD64 sizeOfImage) {
	DWORD64 moduleBase = SymLoadModuleEx(processHandle, fileHandle, imageName, NULL, baseOfImage, static_cast<DWORD>(sizeOfImage), NULL, NULL);
	if (!moduleBase)
		CreateRunTimeError(GetLastErrorMessage());

	_IMAGEHLP_MODULE64 moduleInfo;
	moduleInfo.SizeOfStruct = sizeof(moduleInfo);
	bool success = SymGetModuleInfo64(processHandle, moduleBase, &moduleInfo);
	if (!success)
		CreateRunTimeError(GetLastErrorMessage());

	return moduleInfo;
}

std::size_t GetModuleSize(HMODULE moduleHandler, HANDLE processHandle){
	PE_Parser m_PE_Parser{ moduleHandler, processHandle };
	IMAGE_NT_HEADERS m_Image_Headers = m_PE_Parser.GetImageFileHeaders();
	return m_Image_Headers.OptionalHeader.SizeOfImage;
}

InstructionModifier::InstructionAddress_t GetExecutableStartAddress(HMODULE moduleHandle, HANDLE processHandle) {
	PE_Parser m_PE_Parser{ moduleHandle, processHandle };
	IMAGE_NT_HEADERS m_Image_Headers = m_PE_Parser.GetImageFileHeaders();
	return reinterpret_cast<InstructionModifier::InstructionAddress_t>(m_Image_Headers.OptionalHeader.ImageBase + m_Image_Headers.OptionalHeader.AddressOfEntryPoint);
}