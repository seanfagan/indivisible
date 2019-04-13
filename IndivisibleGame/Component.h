#pragma once
#include "Coordinate.h"
#include "Node.h"
#include <vector>

class Component {
public:
	Component(const Node* node);
	Component(std::vector<Node>& nodes);

	const Node* root() const;
	int size() const;
	void push_back(const Node* node);

private:
	std::vector<const Node*> m_nodes;
};
