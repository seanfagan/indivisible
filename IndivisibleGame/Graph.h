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
	void input_selection(const std::vector<Coordinate>& coords);
	void undo_grouping();
	const Node* get_node(const Coordinate& coord) const;
	const Node* get_node(const int& x, const int& y) const;
	std::vector<std::weak_ptr<const Grouping>> get_groupings() const;
	std::weak_ptr<const Grouping> get_last_grouping() const;
	bool is_complete() const;
	std::map<Node::Party, int> survey_voters() const;
	std::map<Node::Party, int> get_votes() const;
	Node::Party get_winner() const;
	void print() const;  // debug

	static Node::Party get_winner(std::map<Node::Party, int>& votes);

private:
	std::array<std::array<Node, SIZE>, SIZE> nodes;
	std::vector<std::shared_ptr<Grouping>> groupings;

	void initialize(const int& seed);
	void set_adjacency_list(Node& n);
	void dfs_selections(const Node* node, Board<bool>& visited, std::vector<const Node*>& connected) const;
	std::vector<std::string> print_node(const Node& n) const;
	void print_selection_results(const bool& success);
};