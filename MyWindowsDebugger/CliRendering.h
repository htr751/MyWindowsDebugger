#pragma once
#include<Windows.h>
#include<string>
#include"SymbolInfoFactory.h"
#include"DebuggerTasks.h"
#include<iostream>
#include"DebuggerMessages.h"
#include"RenderingInterface.h"

class CliRendering : public RenderingInterface{
	std::wostream& m_woutput;
public:
	CliRendering(std::wostream& woutput)noexcept : m_woutput(woutput){}

	void RenderCpuRegisters(const CONTEXT& renderedContext) override final;
	void RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) override final;
	void RenderStackTraceInformation(const StackTraceData& stackTrace) override final;
	void RenderCurrentInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) override final;
	void RenderDebuggerMessage(const OutputMessage& message) override final;
	void RenderDebuggerMessage(const CreateProcessMessage& message) override final;
	void RenderDebuggerMessage(const CreateThreadMessage& message) override final;
	void RenderDebuggerMessage(const LoadDllMessage& message) override final;
	void RenderDebuggerMessage(const UnLoadDllMessage& message) override final;
	void RenderDebuggerMessage(const ThreadExitMessage& message) override final;
	void RenderDebuggerMessage(const ProcessExitMessage& message) override final;
	void RenderDebuggerMessage(const StopDebuggingMessage& message) override final;
	void RenderDebuggerMessage(const BreakPointMessage& message) override final;
	void RenderDebuggerMessage(const ExceptionMessage& message) override final;
	void RenderSimpleString(const std::string& message) override final;
	void RenderErrorMessage(const std::variant<std::string, std::wstring>& message) override final;
};