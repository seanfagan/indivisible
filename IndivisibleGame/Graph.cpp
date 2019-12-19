#include "Graph.h"
#include <iostream>  // todo: remove dependency
#include <random>
#include <unordered_map>

Graph::Graph() 
	: m_party_a("Alpha", "Alphas", 'a'), m_party_b("Beta", "Betas", 'b')
{
	// Random number generators
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d_party(0, 1);
	std::discrete_distribution<> d_pop({ 6, 3, 1 });

	// For every node
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			Node& n = nodes[y][x];
			// Set initial values
			n.m_coord = Coordinate(x, y);
			set_adjacency_list(n);
			// roll party
			n.m_party = d_party(gen) == 0 ? &m_party_a : &m_party_b;
			// roll population
			n.m_population = d_pop(gen) + 1;
		}
	}
}

const Node* Graph::get_node(const Coordinate& coord) const {
	return &nodes[coord.y][coord.x];
}

const Node* Graph::get_node(const int& x, const int& y) const {
	return &nodes[y][x];
}

std::vector<std::weak_ptr<const Grouping>> Graph::get_groupings() const {
	std::vector<std::weak_ptr<const Grouping>> gs;
	for (const auto& g : groupings) {
		std::weak_ptr<const Grouping> ga = g;
		gs.push_back(ga);
	}
	return gs;
}

std::weak_ptr<const Grouping> Graph::get_last_grouping() const {
	std::weak_ptr<const Grouping> g = groupings.back();
	return g;
}

bool Graph::is_complete() const {
	std::vector<std::vector<const Node*>> selections = get_selections();
	for (const std::vector<const Node*>& s : selections) {
		if (s.front()->m_grouping == NULL) {
			return false;
		}
	}
	return true;
}

Results Graph::get_popular_results() const {
	// Tally all votes from the initial population
	std::unordered_map<Party const*, int> votes;

	// for every node
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			const Node& n = nodes[y][x];
			votes[n.m_party] += n.m_population;
		}
	}
	return Results(votes);
}

Results Graph::get_results() const {
	// Random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> tiebreaker(0, 1);

	// Tally votes from groupings
	std::unordered_map<Party const*, int> votes;
	for (const auto& g : groupings) {
		Results r = g->get_results();
		Party const* winner = r.get_winner();

		if (winner) {
			votes[winner] += 1;
		}
		else {
			// resolve any ties
			Party const* tie_winner = tiebreaker(gen) == 0 ? &m_party_a : &m_party_b;
			std::cout << "[i] Swing district has swung to " << tie_winner->m_name_plural << "!" << std::endl;
			votes[tie_winner] += 1;
		}
	}
	return Results(votes);
}

const Party * Graph::get_party_a() const
{
	return &m_party_a;
}

const Party * Graph::get_party_b() const
{
	return &m_party_b;
}

void Graph::print() const {
	// top line
	std::cout << "   ";
	for (int i = 0; i < nodes.size(); ++i) {
		std::cout << "+-----";
	}
	std::cout << "+" << std::endl;

	// iterate through node rows in reverse (max to min)
	for (int row = nodes.size() - 1; row >= 0; --row) {
		// construct this row's lines of text, node by node
		std::vector<std::string> row_lines(4);
		for (int col = 0; col < nodes.size(); ++col) {
			const Node& n = nodes[row][col];
			std::vector<std::string> node_lines = print_node(n);
			for (int i = 0; i < node_lines.size(); ++i) {
				row_lines[i] += node_lines[i];
			}
		}

		// print row, line by line
		for (int i = 0; i < row_lines.size(); ++i) {
			// print prefix
			switch (i) {
				case 1:
					std::cout << std::to_string(row + 1) + "  |";
					break;
				case 3:
					std::cout << "   +";
					break;
				default: std::cout << "   |";
			}
			std::cout << row_lines[i] << std::endl;  // print line
		}
	}

	// bottom line
	std::cout << "    ";
	for (int i = 0; i < nodes.size(); i++) {
		char index = 'A' + i;
		std::cout << "  " << std::string{ index } + "   ";
	}
	std::cout << std::endl;
}

std::vector<std::string> Graph::print_node(const Node& n) const {
	std::string middle_ln;
	std::string filler_ln;
	{
		if (n.m_grouping == NULL) {
			middle_ln = "  " + std::to_string(n.m_population) + "  ";
			filler_ln = " " + std::string(3, n.m_party->m_ascii) + " ";
		}
		else {
			const Party* w = n.m_grouping->get_results().get_winner();
			if (w) {
				middle_ln = " " + std::string(3, w->m_ascii) + " ";
			}
			else {
				// tie
				middle_ln = " " + std::string(3, '?') + " ";
			}
			filler_ln = std::string(5, ' ');
		}
		// add suffix
		bool connected = false;
		for (auto& neigh : n.m_adjacents) {
			// find eastern neighbor
			if (neigh->m_coord.x > n.m_coord.x) {
				connected = (neigh->m_grouping == n.m_grouping);
			}
		}
		middle_ln += (connected ? " " : "|");
		filler_ln += (connected ? " " : "|");
	}
	std::string bottom_ln;
	{
		// print the separators
		bool connected = false;
		for (auto& neigh : n.m_adjacents) {
			// find vertical neighbor
			if (neigh->m_coord.y < n.m_coord.y) {
				connected = (neigh->m_grouping == n.m_grouping);
			}
		}
		bottom_ln = (connected ? "     " : "-----");
		bottom_ln += "+";
	}
	std::vector<std::string> lines{ filler_ln, middle_ln, filler_ln, bottom_ln };
	return lines;
}

void Graph::input_selection(const std::vector<Coordinate>& coords) {
	bool valid = true;
	std::shared_ptr<Grouping> new_grouping = std::make_shared<Grouping>();

	// Validate the input coordinates
	{
		// check that inputs are within board size
		for (const Coordinate& c : coords) {
			if (c.x < 0 || c.x >= nodes.size() || c.y < 0 || c.y >= nodes.size()) {
				std::cout << "[x] Illegal! " << c << " is outside the valid range." << std::endl;
				valid = false;
				goto stop; // !!! GOTO
			}
		}

		// check that nodes are not already selected
		for (const Coordinate& c : coords) {
			Node& n = nodes[c.y][c.x];
			if (n.m_grouping != NULL) {
				std::cout << "[x] Illegal! " << c << " has already been selected." << std::endl;
				valid = false;
				goto stop;  // !!! GOTO
			}
		}

		// create grouping from selected nodes
		std::vector<Node*> grouped_nodes;
		for (const Coordinate& c : coords) {
			grouped_nodes.push_back(&nodes[c.y][c.x]);
		}
		new_grouping->add_nodes(grouped_nodes);  // modifies nodes!

		// run depth-first-search to find connected components
		std::vector<std::vector<const Node*>> components = get_selections();

		// use connected components for further error checking
		for (const auto& c : components)
		{
			if (c.front()->m_grouping == NULL && c.size() < SIZE) {
				// Case: Unselected area is too small for a valid selection.
				valid = false;
				std::cout << "[x] Illegal! This would create a pocket too small to be selected." << std::endl;
			}
			else if (c.front()->m_grouping != NULL && c.size() != SIZE) {
				// Case: Selection is wrong size.
				valid = false;
				std::cout << "[x] Illegal! Every selection must be " << SIZE << " tiles." << std::endl;
			}
		}
	}
	stop:  // !!! GOTO label

	if (valid) {
		// To save the new Grouping, push a shared pointer of it to the Graph.
		// The grouping will exist so long as at least one shared pointer exists.
		groupings.push_back(new_grouping);
	}

	print_selection_results(valid);
}

void Graph::print_selection_results(const bool& success) {
	if (success) {
		// display info on new grouping
		std::weak_ptr<Grouping const> weak_group = groupings.back();
		std::shared_ptr<Grouping const> group = weak_group.lock();
		if (group) {
			Results r = group->get_results();
			Party const* winner = r.get_winner();

			if (winner) {
				std::cout << "[!] District created! " << winner->m_name_plural << " win, "
					<< r.get_result(winner) << " out of " << r.get_total() << " votes." << std::endl;
			}
			else {
				std::cout << "[!] Swing district created! Polling is tied." << std::endl;
			}
		}
	}
	else {
		// failed to apply selection
		std::cout << "[x] Your selection was not applied." << std::endl;
	}
}

void Graph::undo_grouping() {
	if (groupings.empty()) {
		std::cout << "[i] Nothing to undo my friend." << std::endl;
	}
	else {
		groupings.pop_back();
		std::cout << "[i] Last grouping undone." << std::endl;
	}
}

void Graph::clear_groupings() {
	if (groupings.empty()) {
		std::cout << "[i] Nothing to reset." << std::endl;
	}
	else {
		groupings.clear();
		std::cout << "[i] All groupings cleared." << std::endl;
	}
}

std::vector<std::vector<const Node*>> Graph::get_selections() const {
	Board<bool> visited(0);
	std::vector<std::vector<const Node*>> components;

	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			if (!visited.get(x, y)) {
				// Beginning of a new component found
				const Node* start = get_node(x, y);
				std::vector<const Node*> new_component;
				new_component.push_back(start);

				dfs_selections(start, visited, new_component);

				components.push_back(new_component);
			}
		}
	}

	return components;
}

void Graph::set_adjacency_list(Node& n) {
	// Push the addresses of adjacent nodes to a vector in Node.
	const int& x = n.m_coord.x;
	const int& y = n.m_coord.y;

	if (y > 0) {
		const Node* up = get_node(x, y - 1);
		n.m_adjacents.push_back(up);
	}
	if (y + 1 < nodes.size()) {
		const Node* down = get_node(x, y + 1);
		n.m_adjacents.push_back(down);
	}
	if (x > 0) {
		const Node* left = get_node(x - 1, y);
		n.m_adjacents.push_back(left);
	}
	if (x + 1 < nodes[0].size()) {
		const Node* right = get_node(x + 1, y);
		n.m_adjacents.push_back(right);
	}
}

void Graph::dfs_selections(const Node* node, Board<bool>& visited, std::vector<const Node*>& connected) const {
	// Performs a depth-first-search, recursively building a vector of adjacent nodes with the same value.
	visited.set(node->m_coord, true);

	for (const Node* adj : node->m_adjacents) {
		if (!visited.get(adj->m_coord) && node->m_grouping == adj->m_grouping) {
			connected.push_back(adj);  // add node to connected component
			dfs_selections(adj, visited, connected);
		}
	}
}
