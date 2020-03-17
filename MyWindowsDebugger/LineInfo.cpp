#include"LineInfo.h"
#include<DbgHelp.h>
#include"windowsUtillities.h"

LineInfo::LineInfo(std::string fileName, unsigned long lineNumber) :m_fileName(std::move(fileName)), m_lineNumber(lineNumber) {}
LineInfo::LineInfo(HANDLE processHandle, DWORD64 lineAddress) {
	IMAGEHLP_LINE64 lineInfo = { 0 };
	DWORD displacement = 0;
	if (!SymGetLineFromAddr64(processHandle, lineAddress, &displacement, &lineInfo))
		CreateRunTimeError(GetLastErrorMessage());

	this->m_fileName = lineInfo.FileName;
	this->m_lineNumber = lineInfo.LineNumber;
}