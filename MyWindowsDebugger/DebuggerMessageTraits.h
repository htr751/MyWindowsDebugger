#pragma once
#include<type_traits>
#include"DebuggerMessages.h"
template<typename Message>
struct DebuggerMessageTraits : public std::false_type {};

template<> struct DebuggerMessageTraits<OutputMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<CreateProcessMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<CreateThreadMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<LoadDllMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<UnLoadDllMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<ThreadExitMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<ProcessExitMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<StopDebuggingMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<BreakPointMessage> : public std::true_type {};
template<> struct DebuggerMessageTraits<ExceptionMessage> : public std::true_type {};