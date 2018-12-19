#include <algorithm>
#include <cctype>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
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


struct Coordinate {
	int x, y;

	Coordinate(int xx, int yy)
	{
		x = xx;
		y = yy;
	}

	Coordinate(std::string notation)
	{
		std::tuple<int, int> xy = to_cartesian(notation);
		x = std::get<0>(xy);
		y = std::get<1>(xy);
	}

	static std::tuple<int, int> to_cartesian(std::string notation)
	{
		/** Convert coordinates from board notation ("A1") to cartesian (0, 0). */

		// do math on the ascii values to get integer values
		int a = notation[0] - 'A';
		int b = notation[1] - '1';

		return std::tuple<int, int>(a, b);
	}

	static std::string to_notation(int x, int y)
	{
		/** Convert coordinates from cartesian (0, 0) to board notation ("A1"). */
		char a = 'A' + x;
		char b = '1' + y;

		return std::string{ a, b };
	}
};


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
