#include "Grouping.h"
#include <unordered_map>

Grouping::Grouping() {};

Grouping::~Grouping() {
	// Clear each nodes' grouping property.
	for (auto& n : m_nodes) {
		n->m_grouping = NULL;
	}
}

void Grouping::add_nodes(std::vector<Node*> nodes) {
	m_nodes.insert(m_nodes.end(), nodes.begin(), nodes.end());
	// Set each nodes' grouping property
	for (auto& n : nodes) {
		n->m_grouping = this;
	}
}

Results Grouping::get_results() const {
	// Tally votes from nodes
	std::unordered_map<Party const*, int> votes;
	for (const Node* n : m_nodes) {
		votes[n->m_party] += n->m_population;
	}
	return Results(votes);
}
