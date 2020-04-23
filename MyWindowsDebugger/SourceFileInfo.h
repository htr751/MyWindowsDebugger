#pragma once
#include<string>
#include<string_view>
#include<Windows.h>
#include<map>
#include<vector>
#include<algorithm>

class SourceFileInfo {
public:
	struct SourceFileLineInfo {
		std::string m_objectFileName;
		DWORD m_lineNumber;
		DWORD64 m_lineAddress;

		SourceFileLineInfo(std::string_view objectFileName, DWORD lineNumber, DWORD64 lineAddress) :
			m_objectFileName(objectFileName.data()), m_lineNumber(lineNumber), m_lineAddress(lineAddress) {}

		SourceFileLineInfo() = default;
		SourceFileLineInfo(const SourceFileLineInfo&) = default;
		SourceFileLineInfo(SourceFileLineInfo&&) = default;
		SourceFileLineInfo& operator=(const SourceFileLineInfo&) = default;
		SourceFileLineInfo& operator=(SourceFileLineInfo&&) = default;
		~SourceFileLineInfo() = default;
	};
private:
	std::string m_sourceFilePath;
	DWORD64 m_sourceFileBaseAddress;
	std::map<DWORD, SourceFileLineInfo> lineNumberToSourceFileInfoMap;

public:
	SourceFileInfo(std::string_view sourceFilePath, DWORD64 sourceFileBaseAddress);
	const std::string& GetSourceFilePath() const noexcept;
	DWORD64 GetSourceFileBaseAddress() const noexcept;
	void AddLineInformation(const SourceFileLineInfo& lineInfo);
	const SourceFileLineInfo& GetLineInfo(DWORD lineNumber);

	template<typename Comperator>
	std::vector<SourceFileLineInfo> GetSortedLineInfo(Comperator&& comperator) const {
		std::vector<SourceFileLineInfo> linesInfo{ this->lineNumberToSourceFileInfoMap.size() };
		for (const auto& [lineNumber, lineInfo] : this->lineNumberToSourceFileInfoMap)
			linesInfo.push_back(lineInfo);

		std::sort(linesInfo.begin(), linesInfo.end(), std::forward<Comperator>(comperator));
		return linesInfo;
	}

};