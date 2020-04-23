#pragma once
#include<string>
#include<Windows.h>

struct LineInfo {
	std::string m_fileName;
	unsigned long m_lineNumber;

	LineInfo(std::string fileName, unsigned long LineNumber);
	LineInfo(HANDLE processHandle, DWORD64 lineAddress);
	LineInfo() = default;
	LineInfo(const LineInfo&) = default;
	LineInfo(LineInfo&& other)noexcept : m_fileName(std::move(other.m_fileName)), m_lineNumber(other.m_lineNumber){}
	LineInfo& operator=(const LineInfo&) = default;
	LineInfo& operator=(LineInfo&&) = default;
	~LineInfo() = default;
};