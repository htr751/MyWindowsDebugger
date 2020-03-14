#include"SymbolInfoFactory.h"
#include<memory>
#include"windowsUtillities.h"

SymbolInfoFactory::SymbolInfo::SymbolInfo(const _SYMBOL_INFO& symbolInfo, const IMAGEHLP_LINE64& symbolSourceInfo) :symbolName(symbolInfo.Name){
	this->flags = symbolInfo.Flags;
	this->symbolAddress = symbolInfo.Address;
	this->symbolIndex = symbolInfo.Index;
	this->symbolModuleBaseAddress = symbolInfo.ModBase;
	this->symbolIndexType = symbolInfo.TypeIndex;
	this->symbolRegister = symbolInfo.Register;
	this->symbolScope = symbolInfo.Scope;
	this->symbolSize = symbolInfo.Size;
	this->symbolTag = symbolInfo.Tag;
	this->symbolValue = symbolInfo.Value;
	this->symbolSourceInfo = symbolSourceInfo;
}

std::optional<SymbolInfoFactory::SymbolInfo> SymbolInfoFactory::GetSymbolInfo(HANDLE processHandle, DWORD64 symbolAddr) const {
	constexpr std::size_t symbolInfoSize = sizeof(_SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR);

	DWORD64 displacement = 0;
	std::unique_ptr<BYTE[]> symbolInfoContainer = std::make_unique<BYTE[]>(symbolInfoSize);
	_SYMBOL_INFO* symbolInfo = reinterpret_cast<_SYMBOL_INFO*>(symbolInfoContainer.get());

	std::memset(symbolInfo, 0, symbolInfoSize);
	symbolInfo->SizeOfStruct = sizeof(_SYMBOL_INFO);
	symbolInfo->MaxNameLen = MAX_SYM_NAME;

	bool err = SymFromAddr(processHandle, symbolAddr, &displacement, symbolInfo);
	if (!err) 
		return {};

	IMAGEHLP_LINE64 symbolSourceInfo = { 0 };
	DWORD symbolSourceDisplacement = 0;
	symbolSourceInfo.SizeOfStruct = sizeof(symbolSourceInfo);
	if (!SymGetLineFromAddr64(processHandle, symbolAddr, &symbolSourceDisplacement, &symbolSourceInfo))
		return {};

	return SymbolInfoFactory::SymbolInfo(*symbolInfo, symbolSourceInfo);
}