#pragma once
#include<Windows.h>
#include<string>
#include"SymbolInfoFactory.h"
#include"DebuggerTasks.h"
#include<iostream>
#include"DebuggerMessages.h"

class RenderingInterface {
public:
	virtual void RenderCpuRegisters(const CONTEXT& renderedContext) = 0;
	virtual void RenderSymbolInformation(const SymbolInfoFactory::SymbolInfo& symbolInfo) = 0;
	virtual void RenderStackTraceInformation(const StackTraceData& stackTrace) = 0;
	virtual void RenderDebuggerMessage(const OutputMessage& message) = 0;
	virtual void RenderDebuggerMessage(const CreateProcessMessage& message) = 0;
	virtual void RenderDebuggerMessage(const CreateThreadMessage& message) = 0;
	virtual void RenderDebuggerMessage(const LoadDllMessage& message) = 0;
	virtual void RenderDebuggerMessage(const UnLoadDllMessage& message) = 0;
	virtual void RenderDebuggerMessage(const ThreadExitMessage& message) = 0;
	virtual void RenderDebuggerMessage(const ProcessExitMessage& message) = 0;
	virtual void RenderDebuggerMessage(const StopDebuggingMessage& message) = 0;
	virtual void RenderDebuggerMessage(const BreakPointMessage& message) = 0;
	virtual void RenderDebuggerMessage(const ExceptionMessage& message) = 0;
	virtual void RenderSimpleString(const std::string& message) = 0;
	virtual void RenderErrorMessage(const std::variant<std::string, std::wstring>& message) = 0;
};