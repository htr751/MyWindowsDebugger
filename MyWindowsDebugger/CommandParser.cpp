#include"CommandParser.h"
#include<sstream>
#include<algorithm>
#include<numeric>
#include<iterator>
#include"inputRange.h"

std::list<std::string> TokenizeCommand(const std::string& command) {
	std::istringstream commandStream{ command };
	std::list<std::string> commandTokens;
	InputRange<std::string> inputStream{ commandStream};

	for (const auto& token : inputStream)
		commandTokens.push_back(token);
	return commandTokens;
}