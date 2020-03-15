#include"LineInfo.h"

LineInfo::LineInfo(std::string fileName, int lineNumber) :m_fileName(std::move(fileName)), m_lineNumber(lineNumber) {}