#pragma once
#include<istream>
#include<iterator>

template<typename T>
class InputRange {
	std::istream& m_stream;
public:
	InputRange(std::istream& stream) : m_stream(stream) {}
	template<typename T> friend std::istream_iterator<T> begin(InputRange<T>& range);
	template<typename T> friend std::istream_iterator<T> end(InputRange<T>& range);
};

template<typename T>
std::istream_iterator<T> begin(InputRange<T>& range) {
	return std::istream_iterator<T>{range.m_stream};
}

template<typename T>
std::istream_iterator<T> end(InputRange<T>& range) {
	return std::istream_iterator<T>{};
}