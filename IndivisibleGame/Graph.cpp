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

void Graph::print() const {
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes.size(); ++x) {
			const Node* n = get_node(x, y);
			std::cout << "| " << n->m_selection << " |";
		}
		std::cout << std::endl;
	}
}

bool Graph::input_selection(const std::vector<Coordinate>& coords) {
	// Create a backup of current selections, in case validation fails
	Board<int> temp_selections(0);
	for (const auto& n : nodes) {
		for (const auto& m : n) {
			temp_selections.set(m.m_coord, m.m_selection);
		}
	}

	// Add 1 to graph's selection history
	for (auto& row : nodes) {
		for (auto& node : row) {
			if (node.m_selection > 0) {
				node.m_selection++;
			}
		}
	}

	// Validate the input coordinates
	bool valid = true;
	{ 
		// check that inputs are within board size
		for (const Coordinate& c : coords) {
			if (c.x < 0 || c.x >= nodes.size() || c.y < 0 || c.y >= nodes.size()) {
				std::cout << "[x] Illegal! __ is outside the valid range." << std::endl;
				valid = false;
				goto stop; // !!! exit
			}
		}

		// add new selection to graph
		for (const Coordinate& c : coords) {
			Node& n = nodes[c.y][c.x];

			// check that node is not already selected
			if (n.m_selection != 0) {
				std::cout << "[x] Illegal! __ has already been selected." << std::endl;
				valid = false;
				goto stop;  // !!! exit
			}
			n.m_selection = 1;
		}

		// run depth-first-search to find connected components
		std::vector<Component> components = get_selections();

		// utilize components for error checking
		for (auto c : components)
		{
			// std::cout << "Selection: " << c.root()->m_selection << " | Root: " << c.root()->m_coord.x << "," << c.root()->m_coord.y << " | Size: " << c.size() << std::endl;
			if (c.root()->m_selection == 0 && c.size() < SIZE) {
				// Case: Unselected area is too small for a valid selection.
				valid = false;
				std::cout << "[x] Illegal! This would create a pocket too small to be selected." << std::endl;
			}
			else if (c.root()->m_selection != 0 && c.size() != SIZE) {
				// Case: Selection is too small.
				valid = false;
				std::cout << "[x] Illegal! Every selection must be " << SIZE << " tiles." << std::endl;
			}
		}
	}
	stop:  // !!! GOTO label

	if (!valid) {
		// revert graph's selections back to snapshot
		for (int y = 0; y < nodes.size(); ++y) {
			for (int x = 0; x < nodes.size(); ++x) {
				nodes[y][x].m_selection = temp_selections.get(x, y);
			}
		}
	}

	return valid;
}

std::vector<Component> Graph::get_selections() {
	Board<bool> visited(0);
	std::vector<Component> components;

	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			if (!visited.get(x, y)) {
				// New component found
				const Node* start = get_node(x, y);
				Component new_component(start);

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
			n.m_party = static_cast<Node::Party>(rando() % 2);
			n.m_population = 1;  // todo
			// n.m_selection = rando() % 3;
			set_adjacency_list(n);
		}
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

void Graph::dfs_selections(const Node* node, Board<bool>& visited, Component& connected) {
	// Performs a depth-first-search, recursively building a vector of adjacent nodes with the same value.
	visited.set(node->m_coord, true);

	for (const Node* adj : node->m_adjacents) {
		if (!visited.get(adj->m_coord) && node->m_selection == adj->m_selection) {
			connected.push_back(adj);  // add node to connected component
			dfs_selections(adj, visited, connected);
		}
	}
}
