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
			n.m_selection = rando() % 3;
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
