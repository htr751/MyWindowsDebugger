#pragma once
#include"DebuggerTasks.h"

template<typename Task>
struct DebuggerTaskTraits : public std::false_type {};

template<> struct DebuggerTaskTraits<StackTraceTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<SymbolInforamtionTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<ContextInformationTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<SetBreakPointTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<RemoveBreakPointTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<ContinueTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<StepIntoTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<StepTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<ExitTask> : public std::true_type {};
template<> struct DebuggerTaskTraits<StepOutTask> : public std::true_type {};
