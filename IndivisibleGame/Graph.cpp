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
	std::string indent = "   ";
	std::string hr = "+-----";
	std::string vr = "|";
	std::string party_a = " ... ";
	std::string party_b = " ~~~ ";

	// top line
	std::cout << indent;
	for (int i = 0; i < nodes.size(); ++i) {
		std::cout << hr;
	}
	std::cout << "+" << std::endl;

	// reverse iterate through rows
	for (int row = nodes.size() - 1; row >= 0; --row) {
		for (int ln = 0; ln < 4; ++ln) {
			std::string prefix = "";  // 4 chars
			std::string body = "";  // 5 chars + 1 separator char

			// build prefix
			if (ln == 1) {
				prefix += std::to_string(row + 1) + "  |";
			}
			else if (ln == 3) {
				prefix += "   +";
			}
			else {
				prefix += "   |";
			}

			// iterate through columns
			for (int col = 0; col < nodes.size(); ++col) {
				const Node& n = nodes[row][col];
				
				// build body
				if (ln == 1) {
					// print the population
					body += "  " + std::to_string(n.m_population) + "  ";
				}
				else if (ln == 3) {
					// print the separators
					bool connected = false;
					for (auto& neigh : n.m_adjacents) {
						// find vertical neighbor
						if (neigh->m_coord.y < n.m_coord.y) {
							connected = (neigh->m_grouping == n.m_grouping);
						}
					}

					body += (connected ? "     " : "-----");
				}
				else {
					// print the party
					body += (n.m_party == Node::Party::A ? party_a : party_b);
				}

				// build suffix
				if (ln == 3) {
					body += "+";
				}
				else {
					bool connected = false;
					for (auto& neigh : n.m_adjacents) {
						// find horizontal neighbor
						if (neigh->m_coord.x > n.m_coord.x) {
							connected = (neigh->m_grouping == n.m_grouping);
						}
					}
					body += (connected ? " " : "|");
				}
			}
			std::cout << prefix << body << std::endl;
		}
	}

	// bottom line
	std::cout << "    ";
	for (int i = 0; i < nodes.size(); i++) {
		char index = 'A' + i;
		std::cout << "  " << std::string{ index } + "   ";
	}
	std::cout << std::endl;


	/*
	// old print
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes.size(); ++x) {
			const Node* n = get_node(x, y);
			std::cout << "| " << n->m_selection << " |";
		}
		std::cout << std::endl;
	}
	*/
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

std::vector<std::vector<const Node*>> Graph::get_selections() {
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
			n.m_party = static_cast<Node::Party>(rando() % 2);
			n.m_population = rando() % 3 + 1;
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

void Graph::dfs_selections(const Node* node, Board<bool>& visited, std::vector<const Node*>& connected) {
	// Performs a depth-first-search, recursively building a vector of adjacent nodes with the same value.
	visited.set(node->m_coord, true);

	for (const Node* adj : node->m_adjacents) {
		if (!visited.get(adj->m_coord) && node->m_grouping == adj->m_grouping) {
			connected.push_back(adj);  // add node to connected component
			dfs_selections(adj, visited, connected);
		}
	}
}
