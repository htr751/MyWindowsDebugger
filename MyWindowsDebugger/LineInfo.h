#pragma once
#include<string>

struct LineInfo {
	std::string m_fileName;
	int m_lineNumber;

	LineInfo(std::string fileName, int LineNumber);
	LineInfo()noexcept = default;
	LineInfo(const LineInfo&)noexcept = default;
	LineInfo(LineInfo&&)noexcept = default;
	LineInfo& operator=(const LineInfo&) = default;
	LineInfo& operator=(LineInfo&&) = default;
	~LineInfo() noexcept = default;
};