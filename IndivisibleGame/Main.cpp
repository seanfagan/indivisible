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
	std::cout << "Hi world." << std::endl;

	// create graph
	Graph g(1234);
	std::cout << std::endl;

	while (!g.is_complete()) {
		g.print();

		// get input
		std::cout << "~~~ Input your selection ~~~" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		std::vector<Coordinate> selection = parse_selection(input);

		// apply input to graph
		bool success = g.input_selection(selection);
		if (!success) {
			std::cout << "[!] Your selection was not applied." << std::endl;
			continue;
		}
		std::cout << "[+] District created! ";

		// display info on new grouping
		std::weak_ptr<const Grouping> weak_group = g.get_last_grouping();
		std::shared_ptr<const Grouping> group = weak_group.lock();
		if (group) {
			int p = group->get_population();
			std::map<Node::Party, int> votes = group->get_votes();
			Node::Party winner = Grouping::get_winner(votes);

			if (winner == Node::Party::A) {
				std::cout <<
					"District created! The 'A' Party wins the district, "
					<< votes[Node::Party::A] << " to " << votes[Node::Party::B];
			}
			else if (winner == Node::Party::B) {
				std::cout <<
					"District created! The 'B' Party wins the district, "
					<< votes[Node::Party::B] << " to " << votes[Node::Party::A];
			}
			else {
				std::cout <<
					"Swing district created! Polling is tied, "
					<< votes[Node::Party::A] << " to " << votes[Node::Party::B];
			}
			std::cout << std::endl;
		}
	}
	if (g.is_complete()) {
		std::cout << "Game over!" << std::endl;
		std::map<Node::Party, int> votes = g.get_votes();
		Node::Party winner = Graph::get_winner(votes);
		if (winner == Node::Party::A) {
			std::cout << "The 'A' Party wins, "
				<< votes[Node::Party::A] << " to " << votes[Node::Party::B];
		}
		else {
			std::cout << "The 'B' Party wins, "
				<< votes[Node::Party::B] << " to " << votes[Node::Party::A];
		}
		std::cout << std::endl;
	}

	std::cout << "End" << std::endl;
	std::cin.get();
}
