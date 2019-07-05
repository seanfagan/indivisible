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
	std::pair<Node::Party, int> get_winner() const;
private:
	std::vector<Node*> m_nodes;
};
