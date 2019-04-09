#include "Graph.h"
#include <random>

Graph::Graph(const int& seed) {
	initialize(seed);
}

std::vector<Component> Graph::get_selections() {
	Board<bool> visited(0);
	std::vector<Component> components;

	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			if (!visited.get(x, y)) {
				// New component found
				Coordinate coord(x, y);
				int size = dfs_selections(coord, visited, 0);

				components.push_back(Component(coord, size, nodes[y][x].m_selection));
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
			// Set initial values
			Node& n = nodes[y][x];
			n.m_coord = Coordinate(x, y);
			n.m_party = static_cast<Node::Party>(rando() % 2);
			n.m_population = rando() % 3 + 1;
			n.m_selection = rando() % 3;
			set_adjacency_list(n);
		}
	}
}

void Graph::set_adjacency_list(Node& n) {
	// Push coordinates of adjacent nodes to a vector in Node.
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

int Graph::dfs_selections(const Coordinate& coord, Board<bool>& visited, int size) {
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
				size = dfs_selections(n, visited, size);
			}
		}
	}
	return size;
}
