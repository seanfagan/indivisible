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

intboard ARR = { {
	{2, 2, 0, 1, 1},
	{2, 2, 0, 0, 1},
	{0, 2, 0, 0, 1},
	{0, 1, 0, 0, 1},
	{0, 1, 0, 0, 0},
} };

struct Coordinate {
	int x, y;

	Coordinate() {
		x = 0;
		y = 0;
	}

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

struct Component {
	Coordinate root;
	int size;
	int group;

	Component(Coordinate root, int size, int group)
	{
		root = root;
		size = size;
		group = group;
	}
};

int dfs(const intboard& map, boolboard& visited, int row, int col, int size) {
	/**
		Performs a depth first search at row/column, recursively looking for neighbors with the same value.

		@return The size of the component found.
	*/
	visited[row][col] = true;
	size++;

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
		const int & nrow = n.first;
		const int & ncol = n.second;

		if (!visited[nrow][ncol] && map[nrow][ncol] == map[row][col]) {
			size = dfs(map, visited, nrow, ncol, size);
		}
	}

	return size;
}

int count_components(intboard& map) {
	boolboard visited = { 0 };
	int components = 0;
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			if (!visited[row][col]) {
				components++;
				int depth = dfs(map, visited, row, col, 0);
				const int & group = map[row][col];

				std::cout << "Component of group " << map[row][col] << " was depth " << depth << "!" << std::endl;
				if (group == 0 && depth < SIZE) {
					std::cout << "Illegal: This selection would create an area too small to be selected." << std::endl;
				}
				else if (group != 0 && depth != SIZE) {
					std::cout << "Illegal: Every selection must be " << SIZE << " tiles." << std::endl;
				}
			}  
		}
	}
	return components;
}

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
