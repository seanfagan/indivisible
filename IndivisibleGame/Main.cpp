#include "Graph.h"

#include <algorithm>  // transform
#include <cctype>  // isalnum
#include <iostream>  // cout
#include <regex>  // regex_match
#include <sstream>  // getline
#include <string>  // string
#include <vector>  // vector

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
	// todo: separate IO (commands?)
	// todo: points system
	// todo: cleanup main.cpp
	std::string play = "Y";

	while (play == "Y" || play == "y") {
		// create graph
		Graph g;
		Party const* party_a = g.get_party_a();
		Party const* party_b = g.get_party_b();

		// survey voters and find underdog
		Results pop_results = g.get_popular_results();
		int pop_a = pop_results.get_result(party_a);
		int pop_b = pop_results.get_result(party_b);
		Party const* underdog = pop_a <= pop_b ? party_a : party_b;

		// print details
		std::cout << "[i] " << party_a->m_name_plural << " has " << pop_a << " supporters. "
			<< party_b->m_name_plural << " has " << pop_b << " supporters." << std::endl;
		std::cout << "[!] Divide the people so that " << underdog->m_name_plural << " win!" << std::endl;
		std::cout << std::endl;

		while (!g.is_complete()) {
			g.print();

			// prompt: get input
			std::cout << "   +--= Input your selection =--+" << std::endl;
			std::cout << ">> ";
			std::string input;
			std::getline(std::cin, input);
			std::cout << std::endl;

			if (input == "undo") {
				g.undo_grouping();
			}
			else if (input == "restart" || input == "reset" || input == "clear") {
				g.clear_groupings();
			}
			else {
				std::vector<Coordinate> selection = parse_selection(input);
				g.input_selection(selection);
			}
		}
		// graph has been completed
		std::cout << "[!] Graph complete!" << std::endl;

		// get winner of graph
		Results results = g.get_results();
		Party const* winner = results.get_winner();

		//print winner
		if (winner) {
			std::cout << "[!] " << winner->m_name_plural << " win with " << results.get_result(winner)
				<< " out of "  << results.get_total() << " votes." << std::endl;
		}
		else {
			// tie
			std::cout << "[!] Tie...?" << std::endl; // todo
		}
		

		// did player succeed
		if (winner == underdog) {
			std::cout << "Success! The party with less supporters won! A true underdog story." << std::endl;
		}
		else {
			std::cout << "Failure... The party with the most supporters actually won..." << std::endl;
		}

		// prompt: play again?
		std::cout << "Enter 'Y' to play again" << std::endl;
		std::cout << ">> ";
		std::getline(std::cin, play);
	}

	std::cout << "End" << std::endl;
	std::cin.get();
}
