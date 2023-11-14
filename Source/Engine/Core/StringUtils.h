#pragma once

#include <string>

namespace nc
{
	std::string ToUpper(std::string input);
	std::string ToLower(std::string input);
	bool IsEqualIgnoreCase(std::string input1, std::string input2);
	std::string CreateUnique(std::string input);
}