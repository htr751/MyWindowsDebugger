#include"CliRendering.h"
#include<iostream>
#include<iomanip>
#include<sstream>
#include"overload.h"
#include"Utillities.h"

void CliRendering::RenderCpuRegisters(const CONTEXT& renderedContext) {
	m_woutput << "RAX = 0x" << std::setfill(L'0') <<std::setw(16) << std::hex << renderedContext.Rax << std::endl;
	m_woutput << "RBX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rbx << std::endl;
	m_woutput << "RCX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rcx << std::endl;
	m_woutput << "RDX = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rdx << std::endl;
	m_woutput << "RSI = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rsi << std::endl;
	m_woutput << "RDI = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rdi << std::endl;
	m_woutput << "RIP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rip << std::endl;
	m_woutput << "RSP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rsp << std::endl;
	m_woutput << "RBP = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.Rbp << std::endl;
	m_woutput << "R8 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R8 << std::endl;
	m_woutput << "R9 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R9 << std::endl;
	m_woutput << "R10 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R10 << std::endl;
	m_woutput << "R11 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R11 << std::endl;
	m_woutput << "R12 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R12 << std::endl;
	m_woutput << "R13 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R13 << std::endl;
	m_woutput << "R14 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R14 << std::endl;
	m_woutput << "R15 = 0x" << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.R15 << std::endl;
	m_woutput << "EFLAGS = 0x " << std::setfill(L'0') << std::setw(16) << std::hex << renderedContext.EFlags << std::endl;
}

void CliRendering::RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) {
	m_woutput << std::endl << "symbol " << stringTowstring(symbolInfo.symbolName) << " at address: 0x" << std::setfill(L'0') << std::setw(16) << std::hex << symbolInfo.symbolAddress << std::endl;
	m_woutput << "this symbol is defined at source file " << stringTowstring(symbolInfo.symbolSourceInfo.m_fileName) << ":" << std::dec << symbolInfo.symbolSourceInfo.m_lineNumber << std::endl;

}

void CliRendering::RenderStackTraceInformation(const StackTraceData& stackTrace) {
	for (const auto& stackFrame : stackTrace.stackTrace) 
		m_woutput << "at " << stringTowstring(stackFrame.symbolName) << "( " << stringTowstring(stackFrame.symbolSourceInfo.m_fileName) << ":" <<
			stackFrame.symbolSourceInfo.m_lineNumber << ")" << std::endl;
}

void CliRendering::RenderDebuggerMessage(const OutputMessage& message) {
	std::visit(overload{
			[this](const std::string& message) {this->m_woutput << stringTowstring(message) << std::endl; },
			[this](const std::wstring& message) {this->m_woutput << message << std::endl; }
		}, message.message);
}

void CliRendering::RenderDebuggerMessage(const CreateProcessMessage& message) {
	std::visit(overload{
		[this, &message](const std::string& imageName) {
			this->m_woutput << L"new process created with id: " << message.processID << std::endl;
			this->m_woutput << L"process image is: " << stringTowstring(imageName) << std::endl;
			this->m_woutput << L"process load address is: " << message.loadAddress << std::endl;
		},
		[this, &message](const std::wstring& imageName) {
			this->m_woutput << L"new process created with id: " << message.processID << std::endl;
			this->m_woutput << L"process image is: " << imageName << std::endl;
			this->m_woutput << L"process load address is: " << message.loadAddress << std::endl;
		}
		}, message.imageName);
}

void CliRendering::RenderDebuggerMessage(const CreateThreadMessage& message) {
	this->m_woutput << "new thread created with id: " << message.threadID << std::endl;
	this->m_woutput << "this thread start address is: " << message.startAddress << std::endl;
}

void CliRendering::RenderDebuggerMessage(const LoadDllMessage& message) {
	this->m_woutput << "new dll loaded ad address " << message.loadAddress << std::endl;
	this->m_woutput << "dll name is: " << message.dllName << std::endl;
}

void CliRendering::RenderDebuggerMessage(const UnLoadDllMessage& message) {
	this->m_woutput << "dll " << message.dllName << "unloaded" << std::endl;
}

void CliRendering::RenderDebuggerMessage(const ThreadExitMessage& message) {
	this->m_woutput << L"thread with id " << message.threadID << L"exited with code " << message.exitCode << std::endl;
}

void CliRendering::RenderDebuggerMessage(const ProcessExitMessage& message) {
	this->m_woutput << L"process exited with code " << message.exitCode << std::endl;
}

void CliRendering::RenderDebuggerMessage(const StopDebuggingMessage& message) {
	this->m_woutput << L"the debugger has been stopped, thank you for using my product" << std::endl;
}

void CliRendering::RenderDebuggerMessage(const BreakPointMessage& message) {
	this->m_woutput << L"debuggee encoutred a break point in " << stringTowstring(message.breakPointLine.m_fileName)
		<< ":" << message.breakPointLine.m_lineNumber << std::endl;
}

void CliRendering::RenderDebuggerMessage(const ExceptionMessage& message) {
	this->m_woutput << L"debuggee encoutred an exception with exception code " << message.exceptionCode <<
		L" in " << stringTowstring(message.exceptionLine.m_fileName) << L":" << message.exceptionLine.m_lineNumber << std::endl;
}

void CliRendering::RenderSimpleString(const std::string& message) {
	this->m_woutput << stringTowstring(message) << std::endl;
}

void CliRendering::RenderErrorMessage(const std::variant<std::string, std::wstring>& message){
	std::visit(overload{
		[this](const std::string& message) {
			this->m_woutput << stringTowstring(message) << std::endl;
		},
		[this](const std::wstring& message) {
			this->m_woutput << message << std::endl;
		}
		}, message);
}