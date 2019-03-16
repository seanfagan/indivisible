#include "Coordinate.h"
#include "Component.h"
#include "Board.h"

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

class Graph {
public:
	Graph(const int& seed) {
		initialize(seed);
	}
private:
	struct Node {
		enum Party : char {
			A, B
		};

		Coordinate m_coord;
		Party m_party;
		int m_population = 0;
		std::vector<Coordinate> m_adjacents;
		int m_selection = 0;

		void print() {
			// std::cout << "cor: " << m_coord.x << m_coord.y;
			// std::cout << " prt: " << m_party;
			// std::cout << " pop: " << m_population;
			// std::cout << " adj : " << m_adjacents.size();
			std::cout << " sct: " << m_selection;
		}
	};
	std::array<std::array<Node, SIZE>, SIZE> nodes;

	void initialize(const int& seed) {
		// Random generator
		std::minstd_rand rando;
		rando.seed(seed);

		// For every node
		for (int y = 0; y < nodes.size(); ++y) {
			for (int x = 0; x < nodes[0].size(); ++x) {
				// Set initial values
				Node& n = nodes[y][x];
				n.m_coord = Coordinate(x, y);
				n.m_party = static_cast<Node::Party>(rando() % 2);
				n.m_population = rando() % 3 + 1;
				n.m_selection = rando() % 3;
				set_adjacency_list(n);
				n.print(); // debug
				std::cout << " | ";
			}
			std::cout << std::endl;
		}
	}

	void set_adjacency_list(Node& n) {
		// Push coordinates of adjacent nodes to Node.
		std::vector<Coordinate>& adjs = n.m_adjacents;
		const int& nx = n.m_coord.x;
		const int& ny = n.m_coord.y;

		if (ny > 0) {
			adjs.push_back(Coordinate(nx, ny - 1)); // up
		}
		if (ny + 1 < nodes.size()) {
			adjs.push_back(Coordinate(nx, ny + 1));  // down
		}
		if (nx > 0) {
			adjs.push_back(Coordinate(nx - 1, ny));  // left
		}
		if (nx + 1 < nodes[0].size()) {
			adjs.push_back(Coordinate(nx + 1, ny));  // right
		}
	}
public:
	std::vector<Component> count_components() {
		Board<bool> visited(0);
		std::vector<Component> components;

		for (int y = 0; y < nodes.size(); ++y) {
			for (int x = 0; x < nodes[0].size(); ++x) {
				if (!visited.get(x, y)) {
					// New component found
					Coordinate coord(x, y);
					int size = dfs(coord, visited, 0);

					components.push_back(Component(coord, size, nodes[y][x].m_selection));
				}
			}
		}

		return components;
	}
private:
	int dfs(const Coordinate& coord, Board<bool>& visited, int size) {
		/**
			Performs a depth first search from coordinate, recursively looking for neighbors with the same value.

			@return The size of the component found.
		*/
		visited.set(coord, true);
		size++;

		const Node& me = nodes[coord.y][coord.x];
		for (const Coordinate& n : me.m_adjacents) {
			if (!visited.get(n)) {
				const Node& you = nodes[n.y][n.x];
				if (!visited.get(n) && you.m_selection == me.m_selection) {
					size = dfs(n, visited, size);
				}
			}
		}
		return size;
	}
};

/** Graph stuff */
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

	Graph g(123);
	std::vector<Component> components = g.count_components();

	std::cout << "Yo world!" << std::endl;

	//Board<int> myarr = increment_board(ARR);
	
	//std::vector<Component> components = count_components(ARR);
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
