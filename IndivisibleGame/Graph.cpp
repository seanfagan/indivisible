#include "Graph.h"
#include <random>
#include <iostream>

Graph::Graph(const int& seed) {
	initialize(seed);
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

std::map<Node::Party, int> Graph::survey_voters() const {
	// Get the votes for each party from the initial population
	std::map<Node::Party, int> voters;

	// for every node
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			const Node& n = nodes[y][x];
			voters[n.m_party] += n.m_population;
		}
	}
	return voters;
}

std::map<Node::Party, int> Graph::get_votes() const {
	// tally votes
	std::map<Node::Party, int> votes;
	for (const auto& g : groupings) {
		Node::Party winner = g->get_winner();

		// resolve any ties
		if (winner == Node::Party::unknown) {
			std::minstd_rand rando;
			rando.seed((int)&g);  // use address of grouping as rand seed
			winner = static_cast<Node::Party>(rando() % 2 + 1);
			if (winner == Node::Party::A) {
				std::cout << "[i] Swing district has voted for Party 'A'." << std::endl;
			}
			else {
				std::cout << "[i] Swing district has voted for Party 'B'." << std::endl;
			}
		}

		votes[winner] += 1;
	}
	return votes;
}

Node::Party Graph::get_winner() const {
	// warning: will retally votes
	std::map<Node::Party, int> votes = get_votes();
	return Graph::get_winner(votes);
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
	std::map<Node::Party, std::string> party = {
		{Node::Party::A, " ... "},
		{Node::Party::B, " ~~~ "},
		{Node::Party::unknown, "     "},

	};
	std::string middle_ln;
	std::string filler_ln;
	{
		if (n.m_grouping == NULL) {
			middle_ln = "  " + std::to_string(n.m_population) + "  ";
			filler_ln = party[n.m_party];
		}
		else {
			const Node::Party& w = n.m_grouping->get_winner();
			middle_ln = party[w];
			filler_ln = party[Node::Party::unknown];
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

bool Graph::input_selection(const std::vector<Coordinate>& coords) {
	bool valid = true;
	std::shared_ptr<Grouping> new_grouping = std::make_shared<Grouping>();

	// Validate the input coordinates
	{
		// check that inputs are within board size
		for (const Coordinate& c : coords) {
			if (c.x < 0 || c.x >= nodes.size() || c.y < 0 || c.y >= nodes.size()) {
				std::cout << "[x] Illegal! " << c << " is outside the valid range." << std::endl;
				valid = false;
				goto stop; // !!! GOTO stop
			}
		}

		// check that nodes are not already selected
		for (const Coordinate& c : coords) {
			Node& n = nodes[c.y][c.x];
			if (n.m_grouping != NULL) {
				std::cout << "[x] Illegal! " << c << " has already been selected." << std::endl;
				valid = false;
				goto stop;  // !!! GOTO stop
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
		// todo: document what's going on here with the shared pointers.
		groupings.push_back(new_grouping);
	}

	return valid;
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

void Graph::initialize(const int& seed) {
	// Random generator
	std::minstd_rand rando;
	rando.seed(seed);

	// For every node
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			Node& n = nodes[y][x];
			// Set initial values
			n.m_coord = Coordinate(x, y);
			n.m_party = static_cast<Node::Party>(rando() % 2 + 1);
			n.m_population = rando() % 3 + 1;
			set_adjacency_list(n);
		}
	}
}

// static
Node::Party Graph::get_winner(std::map<Node::Party, int>& votes) {
	// determine winner from votes. ties impossible
	if (votes[Node::Party::A] > votes[Node::Party::B]) {
		return Node::Party::A;
	}
	else {
		return Node::Party::B;
	}
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
