#include"SourceFileInfo.h"

SourceFileInfo::SourceFileInfo(std::string_view sourceFilePath, DWORD64 sourceFileBaseAddress):m_sourceFilePath(sourceFilePath.data()), m_sourceFileBaseAddress(sourceFileBaseAddress) {}

const std::string& SourceFileInfo::GetSourceFilePath() const noexcept {
	return this->m_sourceFilePath;
}

DWORD64 SourceFileInfo::GetSourceFileBaseAddress() const noexcept {
	return this->m_sourceFileBaseAddress;
}

void SourceFileInfo::AddLineInformation(const SourceFileInfo::SourceFileLineInfo& lineInfo) {
	this->lineNumberToSourceFileInfoMap.insert({ lineInfo.m_lineNumber, lineInfo });
}

const SourceFileInfo::SourceFileLineInfo& SourceFileInfo::GetLineInfo(DWORD lineNumber){
	return this->lineNumberToSourceFileInfoMap[lineNumber];
}