#include"LineInfo.h"

LineInfo::LineInfo(std::string fileName, unsigned long lineNumber) :m_fileName(std::move(fileName)), m_lineNumber(lineNumber) {}