#include "StringUtils.h"


std::string nc::ToUpper(std::string input)
{
	// loop every character in string
	for (char& character : input)
	{
		// assign the character to a uppercase version of itself
		character = toupper(character);
	}
	// return the modified input string
	return input;
}

std::string nc::ToLower(std::string input)
{
	// loop every character in string
	for (char& character : input)
	{
		// assign the character to a uppercase version of itself
		character = tolower(character);
	}
	// return the modified input string
	return input;
}

bool nc::IsEqualIgnoreCase(const std::string input1, const std::string input2)
{
	// check for same length
	if (input1.size() != input2.size()) return false;

	// loop the size of the input strings
	for (int i = 0; i < input1.size(); i++)
	{
		// check that the lowercase of both characters are equal
		if (tolower(input1[i]) != tolower(input2[i])) return false;
	}

	return true;
}

std::string nc::CreateUnique(const std::string input)
{
	// create static count
	static uint32_t count = 0;
	
	// return appended count to input and increment
	return input + std::to_string(count++);
	
}
