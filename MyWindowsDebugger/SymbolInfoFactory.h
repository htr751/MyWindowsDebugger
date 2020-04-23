#pragma once
#include<Windows.h>
#include<DbgHelp.h>
#include<string>
#include<optional>
#include"LineInfo.h"

class SymbolInfoFactory {
public:
	struct SymbolInfo {
		ULONG symbolIndexType;
		ULONG symbolIndex;
		ULONG symbolSize;
		ULONG64 symbolModuleBaseAddress;
		ULONG flags;
		ULONG64 symbolValue; //in case the symbol is constant
		ULONG64 symbolAddress;
		ULONG symbolRegister;
		ULONG symbolScope;
		ULONG symbolTag;
		std::string symbolName;
		LineInfo symbolSourceInfo;
		SymbolInfo(const _SYMBOL_INFO& symbolInfo, const LineInfo& symbolSourceInfo);

		SymbolInfo() = default;
		SymbolInfo(const SymbolInfo&) = default;
		SymbolInfo(SymbolInfo&&) = default;
		SymbolInfo& operator=(SymbolInfo&&) = default;
		SymbolInfo& operator=(const SymbolInfo& other) = default;
	};

	SymbolInfoFactory() = default;
	std::optional<SymbolInfoFactory::SymbolInfo> GetSymbolInfo(HANDLE processHandle, DWORD64 symbolAddr) const;
	std::optional<SymbolInfoFactory::SymbolInfo> GetSymbolInfoByName(HANDLE processHandle, const std::string& symbolName) const;
};