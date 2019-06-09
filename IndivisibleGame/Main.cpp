#include "Coordinate.h"
#include "Component.h"
#include "Board.h"
#include "Graph.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#define SIZE 5
using adjboard = std::array<std::array<std::vector<Coordinate>, SIZE>, SIZE>;

Board<int> ARR({ {
	{2, 2, 0, 1, 1},
	{2, 2, 0, 0, 1},
	{0, 2, 0, 0, 1},
	{0, 5, 0, 0, 1},
	{0, 5, 0, 0, 0},
} });

/** User input */
bool is_invalid_char(const char& ch)
{
	/**
		Checks if a char is invalid for tile notation.

		@param ch The char to validate.
		@return A boolean of if the char is invalid.
	*/
	return not std::isalnum(static_cast<unsigned char>(ch));
}

std::vector<Coordinate> parse_selection(const std::string& input)
{
	/**
		Parses the user's input for selecting tiles. Does some cleaning and validation.

		@param input The user's input to parse.
		@return A vector of all the user's coordinate selections.
	*/
	std::stringstream ss(input);
	std::vector<Coordinate> result;
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
		if (!std::regex_match(substr, rgx)) {
			std::cout << "I don't understand '" << substr << "'. Selections must be '[Letter][Number]', like 'A1'." << std::endl;
		} else {
			// substring is valid! (and so are you!)
			Coordinate coor = Coordinate(substr);
			result.push_back(coor);
		};
	}

	return result;
}

int main()
{
	std::cout << "Hi world." << std::endl;

	// create graph
	Graph g(123);

	std::cout << std::endl;

	for (int i = 0; i < 10; ++i) {
		g.print();

		// get input
		std::cout << "~~~ Input your selection ~~~" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		std::vector<Coordinate> selection = parse_selection(input);

		bool success = g.input_selection(selection);
		if (!success) {
			std::cout << "[!] Your selection was not applied." << std::endl;
			// revert graph's selections back to snapshot
		}
		else {
			std::cout << "[+] District created!" << std::endl;
		}
	}

	std::cout << "End" << std::endl;
	std::cin.get();
}
