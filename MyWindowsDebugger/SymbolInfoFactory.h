#pragma once
#include<Windows.h>
#include<DbgHelp.h>
#include<string>
#include<optional>

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

		SymbolInfo(const _SYMBOL_INFO& symbolInfo);
	};

	SymbolInfoFactory() = default;
	std::optional<SymbolInfoFactory::SymbolInfo> GetSymbolInfo(HANDLE processHandle, DWORD64 symbolAddr) const;
};