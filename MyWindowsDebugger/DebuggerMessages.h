#pragma once
#include<Windows.h>
#include<string>
#include<variant>
#include"LineInfo.h"

struct OutputMessage {
	std::variant<std::string, std::wstring> message;
	OutputMessage(std::variant<std::string, std::wstring> message) : message(std::move(message)) {}
	OutputMessage() = delete;
	OutputMessage(OutputMessage&&) = default;
	OutputMessage(const OutputMessage&) = default;
	OutputMessage& operator=(const OutputMessage&) = default;
	OutputMessage& operator=(OutputMessage&&) = default;
	~OutputMessage() = default;
};

struct CreateProcessMessage {
	std::variant<std::string, std::wstring> imageName;
	LPVOID loadAddress;
	DWORD processID;

	CreateProcessMessage(std::variant<std::string, std::wstring> imageName, LPVOID loadAddress, DWORD processID) :
		imageName(std::move(imageName)), loadAddress(loadAddress), processID(processID) {}

	CreateProcessMessage() = delete;
	CreateProcessMessage(const CreateProcessMessage&) = default;
	CreateProcessMessage(CreateProcessMessage&&) = default;
	CreateProcessMessage& operator=(const CreateProcessMessage&) = default;
	CreateProcessMessage& operator=(CreateProcessMessage&&) = default;
	~CreateProcessMessage() = default;
};

struct CreateThreadMessage {
	DWORD threadID;
	DWORD64 startAddress;

	CreateThreadMessage(DWORD threadID, DWORD64 startAddress) :threadID(threadID), startAddress(startAddress) {}
	CreateThreadMessage() = delete;
	CreateThreadMessage(const CreateThreadMessage&) = default;
	CreateThreadMessage(CreateThreadMessage&&) = default;
	CreateThreadMessage& operator=(const CreateThreadMessage&) = default;
	CreateThreadMessage& operator=(CreateThreadMessage&&) = default;
	~CreateThreadMessage() = default;
};

struct LoadDllMessage {
	std::wstring dllName;
	LPVOID loadAddress;

	LoadDllMessage(std::wstring dllName, LPVOID loadAddress) :dllName(std::move(dllName)), loadAddress(loadAddress) {}
	LoadDllMessage() = delete;
	LoadDllMessage(const LoadDllMessage&) = default;
	LoadDllMessage(LoadDllMessage&&) = default;
	LoadDllMessage& operator=(const LoadDllMessage&) = default;
	LoadDllMessage& operator=(LoadDllMessage&&) = default;
	~LoadDllMessage() = default;
};

struct UnLoadDllMessage {
	std::wstring dllName;

	UnLoadDllMessage(std::wstring dllName) : dllName(std::move(dllName)) {}
	UnLoadDllMessage() = delete;
	UnLoadDllMessage(const UnLoadDllMessage&) = default;
	UnLoadDllMessage(UnLoadDllMessage&&) = default;
	UnLoadDllMessage& operator=(const UnLoadDllMessage&) = default;
	UnLoadDllMessage& operator=(UnLoadDllMessage&&) = default;
	~UnLoadDllMessage() = default;
};

struct ThreadExitMessage {
	DWORD threadID;
	DWORD exitCode;

	ThreadExitMessage(DWORD threadID, DWORD exitCode) : threadID(threadID), exitCode(exitCode) {}
	ThreadExitMessage() = delete;
	ThreadExitMessage(const ThreadExitMessage&) = default;
	ThreadExitMessage(ThreadExitMessage&&) = default;
	ThreadExitMessage& operator=(const ThreadExitMessage&) = default;
	ThreadExitMessage& operator=(ThreadExitMessage&&) = default;
	~ThreadExitMessage() = default;
};

struct ProcessExitMessage {
	DWORD exitCode;

	ProcessExitMessage(DWORD exitCode) :exitCode(exitCode) {}
	ProcessExitMessage() = delete;
	ProcessExitMessage(const ProcessExitMessage&) = default;
	ProcessExitMessage(ProcessExitMessage&&) = default;
	ProcessExitMessage& operator=(const ProcessExitMessage&) = default;
	ProcessExitMessage& operator=(ProcessExitMessage&&) = default;
	~ProcessExitMessage() = default;
};

struct StopDebuggingMessage {};

struct BreakPointMessage {
	LineInfo m_breakPointLine;

	BreakPointMessage(LineInfo breakPointInfo) : m_breakPointLine(std::move(breakPointInfo)) {}
	BreakPointMessage() = delete;
	BreakPointMessage(const BreakPointMessage&) = default;
	BreakPointMessage(BreakPointMessage&&) = default;
	BreakPointMessage& operator=(const BreakPointMessage&) = default;
	BreakPointMessage& operator=(BreakPointMessage&&) = default;
	~BreakPointMessage() = default;
};

struct ExceptionMessage {
	LineInfo exceptionLine;
	DWORD exceptionCode;

	ExceptionMessage(LineInfo exceptionLine, DWORD exceptionCode) : exceptionLine(std::move(exceptionLine)), exceptionCode(exceptionCode) {}
	ExceptionMessage() = delete;
	ExceptionMessage(const ExceptionMessage&) = default;
	ExceptionMessage(ExceptionMessage&&) = default;
	ExceptionMessage& operator=(const ExceptionMessage&) = default;
	ExceptionMessage& operator=(ExceptionMessage&&) = default;
	~ExceptionMessage() = default;
};

