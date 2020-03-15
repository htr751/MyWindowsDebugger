#include<thread>
#include"MyWindowsDebugger.h"
#include<iostream>
#include"DebuggerTaskTraits.h"
#include"CommandParser.h"

int main(int argc, char* argv[]) {
	auto list = TokenizeCommand("hello world	how \n are you");
	for (const auto& token : list)
		std::cout << token << std::endl;
}