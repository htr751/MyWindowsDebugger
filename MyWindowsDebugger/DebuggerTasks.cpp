#include"DebuggerTasks.h"

SymbolInforamtionTask::SymbolInforamtionTask(const std::string& symbolName) :symbolName(symbolName) {}
SymbolInforamtionTask::SymbolInforamtionTask(std::string&& symbolName) : symbolName(std::move(symbolName)) {}
const std::string& SymbolInforamtionTask::GetSymbolName() const { return this->symbolName; }

SetBreakPointTask::SetBreakPointTask(const LineInfo& instructionInfo) : instructionSourceInfo(instructionInfo) {}
const LineInfo& SetBreakPointTask::GetInstructionInfo() const { return this->instructionSourceInfo; }

RemoveBreakPointTask::RemoveBreakPointTask(const LineInfo& instructionInfo) : instructionSourceInfo(instructionInfo) {}
const LineInfo& RemoveBreakPointTask::GetInstructionInfo() const { return this->instructionSourceInfo; }