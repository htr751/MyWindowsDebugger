#include<thread>
#include"MyWindowsDebugger.h"
#include<iostream>
#include"DebuggerTaskTraits.h"

int main(int argc, char* argv[]) {
	std::cout << DebuggerTaskTraits<int>::value << std::endl;
}