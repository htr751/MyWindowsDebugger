#pragma once
#include<Windows.h>
#include<string>
#include"SymbolInfoFactory.h"

namespace CliRendering {
	void RenderCpuRegisters(const CONTEXT& renderedContext);
	void RenderModuleLoadSymbolsSuccession(const std::string& moduleName, DWORD64 baseOfDll, bool success);
	void RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo, const IMAGEHLP_LINE64& symbolSourceInfo);
}