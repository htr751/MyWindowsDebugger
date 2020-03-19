#pragma once
#include<Windows.h>
#include<string>
#include"SymbolInfoFactory.h"
#include"DebuggerTasks.h"
#include<iostream>
#include"DebuggerMessages.h"

class CliRendering {
	std::ostream& m_output;
	std::wostream& m_woutput;
public:
	CliRendering(std::ostream& output, std::wostream& woutput)noexcept :m_output(output), m_woutput(woutput){}

	void RenderCpuRegisters(const CONTEXT& renderedContext);
	void RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo);
	void RenderStackTraceInformation(const StackTraceData& stackTrace);
	void RenderOutputDebugMessage(const OutputMessage& message);
	void RenderProcessCreationDebugMessage(const CreateProcessMessage& message);
	void RenderThreadCreationDebugMessage(const CreateThreadMessage& message);
	void RenderLoadDllDebugMessage(const LoadDllMessage& message);
	void RenderUnLoadDllDebugMessage(const UnLoadDllMessage& message);
	void RenderThreadExitDebugMessage(const ThreadExitMessage& message);
	void RenderProcessExitMessage(const ProcessExitMessage& message);
	void RenderStopDebuggingMessage();
	void RenderBreakPointDebugMessage(const BreakPointMessage& message);
	void RenderExceptionDebugMessage(const ExceptionMessage& message);
};