#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#define SIZE 5
using intboard = std::array<std::array<int, SIZE>, SIZE>;
using boolboard = std::array<std::array<bool, SIZE>, SIZE>;

int dfs(intboard map, boolboard& visited, int row, int col, int depth) {
	visited[row][col] = true;
	depth++;

	std::pair<int, int> neighbors[4] = {
		{
			row,
			std::min(col + 1, SIZE - 1) // right
		},
		{
			std::min(row + 1, SIZE - 1), // down
			col
		},
		{
			row,
			std::max(col-1, 0) // left
		},
		{
			std::max(row - 1, 0), // up
			col
		},
	};

	for (const auto& n : neighbors) {
		if (!visited[n.first][n.second] && map[n.first][n.second] == map[row][col]) {
			depth = dfs(map, visited, n.first, n.second, depth);
		}
	}

	return depth;
}

int count_components(intboard map) {
	boolboard visited = { 0 };
	int components = 0;
	for (int row = 0; row < map.size(); row++) {
		for (int col = 0; col < map.size(); col++) {
			if (!visited[row][col]) {
				int depth = dfs(map, visited, row, col, 0);
				components++;
				std::cout << "Component of color " << map[row][col] << " was depth " << depth << "!" << std::endl;
			}  
		}
	}
	return components;
}

intboard ARR = {{
	{2, 2, 0, 1, 1},
	{2, 2, 0, 0, 1},
	{0, 2, 0, 0, 1},
	{0, 1, 0, 0, 1},
	{0, 1, 0, 0, 0},
}};

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
		// do math on the ascii values
		int a = notation[0] - 'A';
		int b = notation[1] - '1';

		return std::tuple<int, int>(a, b);
	}

	static std::string to_notation(int x, int y)
	{
		/** Convert coordinates from cartesian (0, 0) to board notation ("A1"). */
		// do math on the ascii values
		char a = 'A' + x;
		char b = '1' + y;

		return std::string{ a, b };
	}

	std::string Display()
	{
		/** Return a string that represents the coord. */
		return to_notation(x, y);
	}
};


bool is_invalid_char(char ch)
{
	/**
		Checks if a char is invalid for tile notation.

		@param ch The char to validate.
		@return A boolean of if the char is invalid.
	*/
	return not std::isalnum(static_cast<unsigned char>(ch));
}


std::vector<Coordinate> parse_selection(std::string input)
{
	/**
		Parses the user's input for selecting tiles. Does some cleaning and validation.

		@param input The user's input to parse.
		@return A vector of all the user's tile selections.
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

	int components = count_components(ARR);
	std::cout << "Components found: " << components << std::endl;

	/*
	// get input
	std::string input;
	std::getline(std::cin, input);

	// parse input into vector
	std::vector<Coordinate> result = parse_selection(input);

	// print vector
	for (auto i : result)
	{
		std::cout << i.Display() << "|";
	}
	std::cout << std::endl;
	*/

	std::cout << "End" << std::endl;
	std::cin.get();
}
