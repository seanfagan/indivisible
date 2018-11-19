#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


bool is_invalid_notation(char ch)
{
	/**
		Checks if a char is invalid for space notation.

		@param ch The char to validate.
		@return A boolean of if the char is invalid.
	*/
	return not std::isalnum(static_cast<unsigned char>(ch));
}


std::vector<std::string> parse_selection(std::string input)
{
	/**
		Parses the user's input for selecting spaces.

		@param input The user's input to parse.
		@return A vector of all selections the user input.
	*/
	std::stringstream ss(input); // convert input to stringstream
	std::vector<std::string> result;

	// loop through input
	while (ss.good())
	{
		std::string substr; // for each substring
		std::getline(ss, substr, ','); // comma delimiter

		// remove invalid characters
		substr.erase(std::remove_if(substr.begin(), substr.end(), (int(*)(int))is_invalid_notation), substr.end());

		// convert to uppercase
		std::transform(substr.begin(), substr.end(), substr.begin(), ::toupper);

		result.push_back(substr);
	}

	return result;
}


int main()
{
	std::cout << "Hi world." << std::endl;

	// get input
	std::string input;
	std::getline(std::cin, input);

	// parse input into vector
	std::vector<std::string> result = parse_selection(input);

	// print vector
	for (auto i : result)
	{
		std::cout << "|" << i << "|" << std::endl;
	}

	std::cout << "End" << std::endl;
	std::cin.get();
}
