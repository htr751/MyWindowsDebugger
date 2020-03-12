#include"wRunTimeException.h"

wRunTimeException::wRunTimeException(const std::wstring& err) :wException(err) {}
std::wstring wRunTimeException::what() const {
	return this->message;
}