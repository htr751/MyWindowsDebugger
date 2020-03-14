#include"CliRendering.h"
#include<iostream>
#include<iomanip>
#include<sstream>

void CliRendering::RenderCpuRegisters(const CONTEXT& renderedContext) {
	std::wcout << "RAX = 0x" << std::setfill(L'0') <<std::setw(16) << std::hex << renderedContext.Rax << std::endl;
	std::wcout << "RBX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rbx << std::endl;
	std::wcout << "RCX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rcx << std::endl;
	std::wcout << "RDX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rdx << std::endl;
	std::wcout << "RSI = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rsi << std::endl;
	std::wcout << "RDI = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rdi << std::endl;
	std::wcout << "RIP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rip << std::endl;
	std::wcout << "RSP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rsp << std::endl;
	std::wcout << "RBP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rbp << std::endl;
	std::wcout << "R8 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R8 << std::endl;
	std::wcout << "R9 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R9 << std::endl;
	std::wcout << "R10 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R10 << std::endl;
	std::wcout << "R11 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R11 << std::endl;
	std::wcout << "R12 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R12 << std::endl;
	std::wcout << "R13 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R13 << std::endl;
	std::wcout << "R14 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R14 << std::endl;
	std::wcout << "R15 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R15 << std::endl;
	std::wcout << "EFLAGS = 0x " << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.EFlags << std::endl;
}

void CliRendering::RenderModuleLoadSymbolsSuccession(const std::string& moduleName, DWORD64 baseOfDll, bool success) {
	std::cout << "Loaded module " << moduleName.c_str() << " at address 0x" << std::setfill('0') << std::setw(16) << std::hex << baseOfDll;
	if (success)
		std::cout << ", symbols found" << std::endl;
	else
		std::cout << ", no debugging symbols" << std::endl;
}

void CliRendering::RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) {
	std::cout << std::endl << "symbol " << symbolInfo.symbolName << " at address: 0x" << std::setfill('0') << std::setw(16) << std::hex << symbolInfo.symbolAddress << std::endl;
	std::cout << "this symbol is defined at source file " << symbolInfo.symbolSourceInfo.FileName << ":" << std::dec << symbolInfo.symbolSourceInfo.LineNumber << std::endl;

}
