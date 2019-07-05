#include "Grouping.h"

Grouping::Grouping() {};

Grouping::Grouping(std::vector<Node*> nodes)
	: m_nodes(nodes) {
	add_nodes(nodes);
}

Grouping::~Grouping() {
	// Clear all nodes' grouping property.
	for (auto& n : m_nodes) {
		n->m_grouping = NULL;
	}
}

void Grouping::add_nodes(std::vector<Node*> nodes) {
	m_nodes.insert(m_nodes.end(), nodes.begin(), nodes.end());
	// Set all nodes' grouping property
	for (auto& n : nodes) {
		n->m_grouping = this;
	}
}

int Grouping::get_population() const {
	int pop = 0;
	for (const auto& n : m_nodes) {
		pop += n->m_population;
	}
	return pop;
}

std::map<Node::Party, int> Grouping::get_votes() const {
	std::map<Node::Party, int> votes;
	for (const auto& n : m_nodes) {
		votes[n->m_party] += n->m_population;
	}
	return votes;
}

std::pair<Node::Party, int> Grouping::get_winner() const {
	std::map<Node::Party, int> votes = get_votes();
	Node::Party winner_party;
	int winner_votes;

	if (votes[Node::A] > votes[Node::B]) {
		winner_party = Node::A;
		winner_votes = votes[Node::A];
	}
	else if (votes[Node::B] > votes[Node::A]) {
		winner_party = Node::B;
		winner_votes = votes[Node::B];
	}
	else {
		// tie
		winner_party = Node::unknown;
		winner_votes = votes[Node::A];
	}
	return std::pair<Node::Party, int>(winner_party, winner_votes);
}
