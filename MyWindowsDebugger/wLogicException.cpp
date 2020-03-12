#include"wLogicException.h"

wLogicException::wLogicException(const std::wstring& err) :wException(err) {}
std::wstring wLogicException::what() const { return this->message; }