#include "Component.h"

Component::Component(const Node* node) {
	m_nodes.push_back(node);
}

Component::Component(std::vector<Node>& nodes) {
	for (Node &n : nodes) {
		m_nodes.push_back(&n);
	}
}

const Node* Component::root() const {
	return m_nodes.front();
}

int Component::size() const {
	return m_nodes.size();
}

void Component::push_back(const Node* node) {
	m_nodes.push_back(node);
}
