#pragma once
#include "Node.h"
#include "Results.h"
#include <vector>

class Grouping {
public:
	// Constructors
	Grouping();
	~Grouping();

	// Methods
	void add_nodes(std::vector<Node*> nodes);
	Results get_results() const;
private:
	// Vars
	std::vector<Node*> m_nodes;
};
