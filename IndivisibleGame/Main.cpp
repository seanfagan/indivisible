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

	Coordinate()
		: x(0), y(0) {}

	Coordinate(const int& x, const int& y)
		: x(x), y(y) {}

	Coordinate(const std::string& notation)
	{
		std::tuple<int, int> xy = to_cartesian(notation);
		x = std::get<0>(xy);
		y = std::get<1>(xy);
	}

	int col() const { return x; }
	int row() const { return y; }

	static std::tuple<int, int> to_cartesian(const std::string& notation)
	{
		/** Convert coordinates from board notation ("A1") to cartesian (0, 0). */
		int a = notation[0] - 'A';
		int b = notation[1] - '1';

		return std::tuple<int, int>(a, b);
	}

	static std::string to_notation(const int& x, const int& y)
	{
		/** Convert coordinates from cartesian (0, 0) to board notation ("A1"). */
		char a = 'A' + x;
		char b = '1' + y;

		return std::string{ a, b };
	}
};

struct Component {
	Coordinate root;
	int size;
	int group;

	Component(const Coordinate& root, const int& size, const int& group)
		: root(root), size(size), group(group) {}
};

int dfs(const intboard& map, boolboard& visited, const Coordinate& coord, int size) {
	/**
		Performs a depth first search from coordinate, recursively looking for neighbors with the same value.

		@return The size of the component found.
	*/
	visited[coord.row()][coord.col()] = true;
	size++;

	const int& x = coord.x;
	const int& y = coord.y;

	Coordinate neighbors[4] = {
		Coordinate(std::min(x + 1, SIZE - 1), y),  // right
		Coordinate(x, std::min(y + 1, SIZE - 1)),  // down
		Coordinate(std::max(x - 1, 0), y),  // left
		Coordinate(x, std::max(y - 1, 0)),  // up
	};

	for (Coordinate& n : neighbors) {
		if (!visited[n.row()][n.col()] && map[n.row()][n.col()] == map[coord.row()][coord.col()]) {
			size = dfs(map, visited, n, size);
		}
	}

	return size;
}

std::vector<Component> count_components(const intboard& map) {
	boolboard visited = { 0 };
	std::vector<Component> components;
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			if (!visited[row][col]) {
				// New component found
				Coordinate coord = Coordinate(col, row);
				int size = dfs(map, visited, coord, 0);
				const int & group = map[coord.row()][coord.col()];

				components.push_back(Component(coord, size, group));
			}  
		}
	}
	return components;
}

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
