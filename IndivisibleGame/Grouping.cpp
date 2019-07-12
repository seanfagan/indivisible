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
	// tally votes
	std::map<Node::Party, int> votes;
	for (const auto& n : m_nodes) {
		votes[n->m_party] += n->m_population;
	}
	return votes;
}

Node::Party Grouping::get_winner() const {
	// warning: will retally votes
	std::map<Node::Party, int> votes = get_votes();
	return Grouping::get_winner(votes);
}

// static
Node::Party Grouping::get_winner(std::map<Node::Party, int> votes) {
	// determine winner from votes
	if (votes[Node::Party::A] > votes[Node::Party::B]) {
		return Node::Party::A;
	}
	else if (votes[Node::Party::B] > votes[Node::Party::A]) {
		return Node::Party::B;
	}
	else {
		return Node::Party::unknown;  // tie
	}
}
