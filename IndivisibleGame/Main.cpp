#include "Coordinate.h"
#include "Component.h"
#include "Board.h"

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
using adjboard = std::array<std::array<std::vector<Coordinate>, SIZE>, SIZE>;

Board<int> ARR({ {
	{2, 2, 0, 1, 1},
	{2, 2, 0, 0, 1},
	{0, 2, 0, 0, 1},
	{0, 5, 0, 0, 1},
	{0, 5, 0, 0, 0},
} });

/** Graph stuff */
int dfs(const Board<int>& map, const adjboard& adjs, Board<bool>& visited, const Coordinate& coord, int size) {
	/**
		Performs a depth first search from coordinate, recursively looking for neighbors with the same value.

		@return The size of the component found.
	*/
	visited.set(coord, true);
	size++;

	std::vector<Coordinate> me = adjs[coord.y][coord.x];
	for (Coordinate& n : me) {
		if (!visited.get(n) && map.get(n) == map.get(coord)) {
			size = dfs(map, adjs, visited, n, size);
		}
	}

	return size;
}

std::vector<Component> count_components(const Board<int>& map) {
	Board<bool> visited(0);
	std::vector<Component> components;

	// -- adjacents
	adjboard adjs;  // todo: size

	for (int y = 0; y < adjs.size(); ++y) {
		for (int x = 0; x < adjs[0].size(); ++x) {
			std::vector<Coordinate>& cell = adjs[y][x];

			if (y > 0) {
				cell.push_back(Coordinate(x, y - 1));  // up
			}
			if (y + 1 < adjs.size()) {
				cell.push_back(Coordinate(x, y + 1));  // down
			}
			if (x > 0) {
				cell.push_back(Coordinate(x - 1, y));  // left
			}
			if (x + 1 < adjs[0].size()) {
				cell.push_back(Coordinate(x + 1, y));  // right
			}
		}
	}

	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			if (!visited.get(col, row)) {
				// New component found
				Coordinate coord = Coordinate(col, row);
				int size = dfs(map, adjs, visited, coord, 0);

				components.push_back(Component(coord, size, map.get(coord)));
			}  
		}
	}

	return components;
}

Board<int> increment_board(Board<int> board) {
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			const int& val = board.get(col, row);
			if (val) {
				board.set(col, row, val + 1);
			}
		}
	}

	return board;
}

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

	//Board<int> myarr = increment_board(ARR);

	std::vector<Component> components = count_components(ARR);
	for (auto c : components)
	{
		std::cout << "Group: " << c.group << " | Root: " << c.root.x << "," << c.root.y << " | Size: " << c.size << std::endl;
		if (c.group == 0 && c.size < SIZE) {
			std::cout << "Illegal: This selection would create an area too small to be selected." << std::endl;
		}
		else if (c.group != 0 && c.size != SIZE) {
			std::cout << "Illegal: Every selection must be " << SIZE << " tiles." << std::endl;
		}
	}

	/*
	// get input
	std::string input;
	std::getline(std::cin, input);

	// parse input into vector
	std::vector<Coordinate> result = parse_selection(input);

	std::cout << std::endl;
	*/

	std::cout << "End" << std::endl;
	std::cin.get();
}
