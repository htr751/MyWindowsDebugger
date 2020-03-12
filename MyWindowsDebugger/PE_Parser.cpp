#include"PE_Parser.h"
#include<memory>
#include"windowsUtillities.h"
static constexpr std::size_t SIZEOF_DOS_HEADER = sizeof(IMAGE_DOS_HEADER);
static constexpr std::size_t SIZEOF_NT_HEADERS = sizeof(IMAGE_NT_HEADERS);

PE_Parser::PE_Parser(HMODULE loadAddress, HANDLE processHandle)noexcept : m_loadAddress(loadAddress), m_processHandle(processHandle) {}

IMAGE_DOS_HEADER PE_Parser::GetDosHeader() const {
	IMAGE_DOS_HEADER header;
	std::size_t numberOfBytesRead = 0;

	bool err = ReadProcessMemory(this->m_processHandle, this->m_loadAddress, &header, SIZEOF_DOS_HEADER, &numberOfBytesRead);
	if (!err || numberOfBytesRead != SIZEOF_DOS_HEADER)
		CreateRunTimeError(GetLastErrorMessage());
	return header;
}

IMAGE_NT_HEADERS PE_Parser::GetImageFileHeaders()const {
	IMAGE_DOS_HEADER m_DOS_Header = this->GetDosHeader();
	IMAGE_NT_HEADERS m_Headers = { 0 };
	std::size_t numberOfBytesRead = 0;

	bool err = ReadProcessMemory(this->m_processHandle, this->m_loadAddress, &m_Headers, SIZEOF_NT_HEADERS, &numberOfBytesRead);
	if (!err || numberOfBytesRead != SIZEOF_NT_HEADERS)
		CreateRunTimeError(GetLastErrorMessage());

	return m_Headers;
}