#pragma once
#include<string>

struct LineInfo {
	std::string m_fileName;
	unsigned long m_lineNumber;

	LineInfo(std::string fileName, unsigned long LineNumber);
	LineInfo() = default;
	LineInfo(const LineInfo&) = default;
	LineInfo(LineInfo&&) = default;
	LineInfo& operator=(const LineInfo&) = default;
	LineInfo& operator=(LineInfo&&) = default;
	~LineInfo() = default;
};