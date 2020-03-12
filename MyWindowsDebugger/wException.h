#pragma once
#include<string>
class wException {
protected:
	std::wstring message;
public:
	wException(const std::wstring& err);
	virtual std::wstring what() const = 0;
	virtual ~wException() {}
};