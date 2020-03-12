#pragma once
#include"wException.h"
class wLogicException :wException {
public:
	wLogicException(const std::wstring& err);
	virtual std::wstring what()const override;
	~wLogicException() = default;
};