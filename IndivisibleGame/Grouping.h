#pragma once
#include "Node.h"
#include <map>
#include <vector>

class Grouping {
public:
	Grouping();
	Grouping(std::vector<Node*> nodes);
	~Grouping();

	void add_nodes(std::vector<Node*> nodes);
	int get_population() const;
	std::map<Node::Party, int> get_votes() const;
	Node::Party get_winner() const;

	static Node::Party get_winner(std::map<Node::Party, int> votes);
private:
	std::vector<Node*> m_nodes;
};
