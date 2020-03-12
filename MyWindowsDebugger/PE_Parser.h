#pragma once
#include<Windows.h>
#include<winnt.h>

class PE_Parser {
	HMODULE m_loadAddress;
	HANDLE m_processHandle; //this is an handle to the process in which the module is loaded

public:
	PE_Parser(HMODULE loadAddress, HANDLE processHandle)noexcept;
	IMAGE_DOS_HEADER GetDosHeader() const;
	IMAGE_NT_HEADERS GetImageFileHeaders()const;
};