#include"CliRendering.h"
#include<iostream>
#include<iomanip>
#include<sstream>
#include"overload.h"

void CliRendering::RenderCpuRegisters(const CONTEXT& renderedContext) {
	m_output << "RAX = 0x" << std::setfill('0') <<std::setw(16) << std::hex << renderedContext.Rax << std::endl;
	m_output << "RBX = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rbx << std::endl;
	m_output << "RCX = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rcx << std::endl;
	m_output << "RDX = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rdx << std::endl;
	m_output << "RSI = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rsi << std::endl;
	m_output << "RDI = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rdi << std::endl;
	m_output << "RIP = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rip << std::endl;
	m_output << "RSP = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rsp << std::endl;
	m_output << "RBP = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.Rbp << std::endl;
	m_output << "R8 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R8 << std::endl;
	m_output << "R9 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R9 << std::endl;
	m_output << "R10 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R10 << std::endl;
	m_output << "R11 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R11 << std::endl;
	m_output << "R12 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R12 << std::endl;
	m_output << "R13 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R13 << std::endl;
	m_output << "R14 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R14 << std::endl;
	m_output << "R15 = 0x" << std::setfill('0') << std::setw(16) << std::hex << renderedContext.R15 << std::endl;
	m_output << "EFLAGS = 0x " << std::setfill('0') << std::setw(16) << std::hex << renderedContext.EFlags << std::endl;
}

void CliRendering::RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) {
	m_output << std::endl << "symbol " << symbolInfo.symbolName << " at address: 0x" << std::setfill('0') << std::setw(16) << std::hex << symbolInfo.symbolAddress << std::endl;
	m_output << "this symbol is defined at source file " << symbolInfo.symbolSourceInfo.m_fileName << ":" << std::dec << symbolInfo.symbolSourceInfo.m_lineNumber << std::endl;

}

void CliRendering::RenderStackTraceInformation(const StackTraceData& stackTrace) {
	for (const auto& stackFrame : stackTrace.stackTrace) 
		m_output << "at " << stackFrame.symbolName << "( " << stackFrame.symbolSourceInfo.m_fileName << ":" <<
			stackFrame.symbolSourceInfo.m_lineNumber << ")" << std::endl;
}

void CliRendering::RenderOutputDebugMessage(const OutputMessage& message) {
	std::visit(overload{
			[this](const std::string& message) {this->m_output << message << std::endl; },
			[this](const std::wstring& message) {this->m_woutput << message << std::endl; }
		}, message.message);
}

