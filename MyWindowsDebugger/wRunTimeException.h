#pragma once
#include"wException.h"

class wRunTimeException : public wException {
public:
	wRunTimeException(const std::wstring& err);
	virtual std::wstring what() const override;
	virtual ~wRunTimeException() = default;
};