#include <algorithm>
#include <cctype>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>


bool is_invalid_char(char ch)
{
	/**
		Checks if a char is invalid for tile notation.

		@param ch The char to validate.
		@return A boolean of if the char is invalid.
	*/
	return not std::isalnum(static_cast<unsigned char>(ch));
}

bool is_valid_notation(std::string selection)
{
	std::regex rgx("^[A-Z]\d$");

	return std::regex_match(selection, rgx);
} 


std::vector<std::string> parse_selection(std::string input)
{
	/**
		Parses the user's input for selecting tiles. Does some cleaning and validating.

		@param input The user's input to parse.
		@return A vector of all the user's tile selections.
	*/
	std::stringstream ss(input); // convert input to stringstream
	std::vector<std::string> result;
	std::regex rgx("[A-Z]\\d"); // regex of valid notation

	while (ss.good())
	{
		// separate a substring
		std::string substr;
		std::getline(ss, substr, ','); // comma delimiter

		// strip out invalid characters
		substr.erase(std::remove_if(substr.begin(), substr.end(), is_invalid_char), substr.end());

		// convert to uppercase
		std::transform(substr.begin(), substr.end(), substr.begin(), ::toupper);

		// run against regex
		if (std::regex_match(substr, rgx)) {
			result.push_back(substr);
		} else {
			std::cout << "I don't understand '" << substr << "'. Selections must be '[Letter][Number]', like 'A1'." << std::endl;
		};
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
