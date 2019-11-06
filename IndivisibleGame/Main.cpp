#include "Coordinate.h"
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
	// todo: win state
	// todo: cleanup main.cpp
	// todo: messenger
	std::string play = "Y";

	while (play == "Y") {
		// create graph
		Graph g(1234);

		// survey voters
		std::map<Node::Party, int> survey = g.survey_voters();
		std::cout << "[i] Party A has " << survey[Node::Party::A] << " supporters." << std::endl;
		std::cout << "[i] Party B has " << survey[Node::Party::B] << " supporters." << std::endl;
		std::cout << std::endl;

		while (!g.is_complete()) {
			g.print();

			// get input
			std::cout << "   +--= Input your selection =--+" << std::endl;
			std::cout << ">> ";
			std::string input;
			std::getline(std::cin, input);
			std::cout << std::endl;

			if (input == "undo") {
				g.undo_grouping();
			}
			else {
				std::vector<Coordinate> selection = parse_selection(input);
				g.input_selection(selection);
			}
		}
		// graph has been completed
		std::cout << "[i] Area complete!" << std::endl;

		// get winner of graph
		std::map<Node::Party, int> votes = g.get_votes();
		Node::Party winner = Graph::get_winner(votes);
		if (winner == Node::Party::A) {
			std::cout << "[! ]Party A wins, "
				<< votes[Node::Party::A] << " to " << votes[Node::Party::B];
		}
		else {
			std::cout << "[!] Party B wins, "
				<< votes[Node::Party::B] << " to " << votes[Node::Party::A];
		}
		std::cout << std::endl;

		std::cout << "Enter 'Y' to play again" << std::endl;
		std::cout << ">> ";
		std::getline(std::cin, play);
	}

	std::cout << "End" << std::endl;
	std::cin.get();
}
