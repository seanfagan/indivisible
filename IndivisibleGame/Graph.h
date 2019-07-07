#pragma once
#include "Board.h"
#include "Node.h"
#include "Grouping.h"
#include <array>
#include <memory>
#include <vector>
#define SIZE 5 // todo

class Graph {
public:
	Graph(const int& seed);
	std::vector<std::vector<const Node*>> get_selections() const;
	bool input_selection(const std::vector<Coordinate>& coords);
	const Node* get_node(const Coordinate& coord) const;
	const Node* get_node(const int& x, const int& y) const;
	std::vector<std::weak_ptr<const Grouping>> get_groupings() const;
	std::weak_ptr<const Grouping> get_last_grouping() const;
	bool is_complete() const;
	void print() const;  // debug

private:
	std::array<std::array<Node, SIZE>, SIZE> nodes;
	std::vector<std::shared_ptr<Grouping>> groupings;

	void initialize(const int& seed);
	void set_adjacency_list(Node& n);
	void dfs_selections(const Node* node, Board<bool>& visited, std::vector<const Node*>& connected) const;
};