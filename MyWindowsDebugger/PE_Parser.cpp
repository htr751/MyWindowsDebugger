#include"PE_Parser.h"
#include<memory>
#include"windowsUtillities.h"
#include<iostream>
#include"Utillities.h"
#include<catch.hpp>

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

	bool err = ReadProcessMemory(this->m_processHandle, (LPCVOID)((DWORD64)this->m_loadAddress + m_DOS_Header.e_lfanew), &m_Headers, SIZEOF_NT_HEADERS, &numberOfBytesRead);
	if (!err || numberOfBytesRead != SIZEOF_NT_HEADERS)
		CreateRunTimeError(GetLastErrorMessage());

	return m_Headers;
}

TEST_CASE("PE_Parser check", "[PE_Parser][GetDosHeader]") {
	unique_handle processHandle{ GetCurrentProcess() };
	HMODULE moduleHandle = GetModuleHandleA(NULL);
	PE_Parser m_parser{ moduleHandle, processHandle.getHandle() };

	SECTION("GetDosHeader check") {
		auto DosHeader = m_parser.GetDosHeader();
		REQUIRE(DosHeader.e_magic == IMAGE_DOS_SIGNATURE);
	}
	SECTION("GetImageFileHeaders check") {
		auto imageHeaders = m_parser.GetImageFileHeaders();
		REQUIRE(imageHeaders.Signature == IMAGE_NT_SIGNATURE);
	}
}