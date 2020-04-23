#include"SymbolInfoFactory.h"
#include<memory>
#include"windowsUtillities.h"
#include"LineInfo.h"
constexpr static std::size_t symbolInfoSize = sizeof(_SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR);
SymbolInfoFactory::SymbolInfo::SymbolInfo(const _SYMBOL_INFO& symbolInfo, const LineInfo& symbolSourceInfo) :symbolName(symbolInfo.Name), symbolSourceInfo(symbolSourceInfo){
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
}

std::optional<SymbolInfoFactory::SymbolInfo> SymbolInfoFactory::GetSymbolInfo(HANDLE processHandle, DWORD64 symbolAddr) const {

	DWORD64 displacement = 0;
	std::unique_ptr<BYTE[]> symbolInfoContainer = std::make_unique<BYTE[]>(symbolInfoSize);
	_SYMBOL_INFO* symbolInfo = reinterpret_cast<_SYMBOL_INFO*>(symbolInfoContainer.get());

	std::memset(symbolInfo, 0, symbolInfoSize);
	symbolInfo->SizeOfStruct = sizeof(_SYMBOL_INFO);
	symbolInfo->MaxNameLen = MAX_SYM_NAME;

	bool err = SymFromAddr(processHandle, symbolAddr, &displacement, symbolInfo);
	if (!err) 
		return {};
	try {
		LineInfo symbolLineInfo{ processHandle, symbolAddr };
		return SymbolInfoFactory::SymbolInfo(*symbolInfo, symbolLineInfo);
	}
	catch (...) { return {}; }
}

std::optional<SymbolInfoFactory::SymbolInfo> SymbolInfoFactory::GetSymbolInfoByName(HANDLE processHandle, const std::string& symbolName) const {
	std::unique_ptr<BYTE[]> symbolInfoContainer = std::make_unique<BYTE[]>(symbolInfoSize);
	_SYMBOL_INFO* symbolInfo = reinterpret_cast<_SYMBOL_INFO*>(symbolInfoContainer.get());

	std::memset(symbolInfo, 0, symbolInfoSize);
	symbolInfo->SizeOfStruct = sizeof(_SYMBOL_INFO);
	symbolInfo->MaxNameLen = MAX_SYM_NAME;

	if (!SymFromName(processHandle, symbolName.c_str(), symbolInfo))
		return {};
	try {
		LineInfo symbolLineInfo{ processHandle, symbolInfo->Address };
		return SymbolInfoFactory::SymbolInfo(*symbolInfo, symbolLineInfo);
	}
	catch (...) { return {}; }
}