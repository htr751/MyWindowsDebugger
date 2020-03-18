#include<thread>
#include"MyWindowsDebugger.h"
#include<iostream>
#include"DebuggerTaskTraits.h"
#include"CommandParser.h"
#include<variant>
#include"overload.h"
int main(int argc, char* argv[]) {
	std::variant<std::string, int> m_variant{ "hello world" };
	try {
		std::visit(overload{
			[](auto&& param) {throw std::runtime_error("error with param"); }
			}, m_variant);
	}
	catch (std::runtime_error & err) { std::cout << err.what() << std::endl; }

}