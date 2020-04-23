#pragma once
#include<locale>
#include<codecvt>
#include<string>
#include<optional>
#include<queue>

using InstructionAddress_t = void*;

std::string wstringTostring(const std::wstring& str);
std::wstring stringTowstring(const std::string& str);

//throws runtime error  with message as its message
void CreateRunTimeError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage = std::wstring(L"unknown error type"));

//throws logic error with message as its message
void CreateLogicError(const std::optional<std::wstring>& optionalMessage, const std::wstring& alternativeMessage = std::wstring(L"unknown error type"));

template<typename Data>
void emptyQueue(std::queue<Data>& queueToEmpty) {
	while (!queueToEmpty.empty())
		queueToEmpty.pop();
}

template<typename Data>
std::vector<Data> createVectorFromQueue(std::queue<Data>& queue) {
	std::vector<Data> result;
	result.reserve(queue.size());
	while (!queue.empty()) {
		result.push_back(queue.front());
		queue.pop();
	}
	return result;
}
