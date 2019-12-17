#pragma once
#include "Node.h"
#include "Results.h"
#include <map>
#include <vector>

class Grouping {
public:
	Grouping();
	Grouping(std::vector<Node*> nodes);
	~Grouping();

	void add_nodes(std::vector<Node*> nodes);
	Results get_results() const;
private:
	std::vector<Node*> m_nodes;
};
