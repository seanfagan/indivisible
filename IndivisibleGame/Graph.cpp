#include "Graph.h"

#include <iostream>  // todo: remove dependency
#include <random>
#include <unordered_map>

Graph::Graph() 
	: m_party_a("Alpha", "Alphas", 'i'), m_party_b("Beta", "Betas", '.')
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
			votes[tie_winner] += 1;
			TieBrokenEvent evt = TieBrokenEvent(tie_winner);
			m_event_registry.process_event(evt);
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
		GroupingSucceededEvent evt = GroupingSucceededEvent(new_grouping);
		m_event_registry.process_event(evt);
	}
	else {
		GroupingFailedEvent evt = GroupingFailedEvent();
		m_event_registry.process_event(evt);
	}
}

void Graph::undo_grouping() {
	if (groupings.empty()) {
		// nothing to undo
		GroupingUndoFailedEvent evt = GroupingUndoFailedEvent();
		m_event_registry.process_event(evt);
	}
	else {
		groupings.pop_back(); // undo last grouping
		GroupingUndoSucceededEvent evt = GroupingUndoSucceededEvent();
		m_event_registry.process_event(evt);
	}
}

void Graph::clear_groupings() {
	if (groupings.empty()) {
		// nothing to clear
		GroupingClearFailedEvent evt = GroupingClearFailedEvent();
		m_event_registry.process_event(evt);
	}
	else {
		groupings.clear(); // clear all groupings
		GroupingClearSucceededEvent evt = GroupingClearSucceededEvent();
		m_event_registry.process_event(evt);
	}
}

std::array<std::array<Node const*, SIZE>, SIZE> Graph::get_nodes() const
{
	std::array<std::array<Node const*, SIZE>, SIZE> arr;
	for (int y = 0; y < nodes.size(); ++y) {
		for (int x = 0; x < nodes[0].size(); ++x) {
			arr[y][x] = get_node(x, y);
		}
	}
	return arr;
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
