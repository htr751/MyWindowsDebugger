#pragma once
#include<Windows.h>
#include<string>

namespace CliRendering {
	void RenderCpuRegisters(const CONTEXT& renderedContext);
	void RenderModuleLoadSymbolsSuccession(const std::string& moduleName, DWORD64 baseOfDll, bool success);
}