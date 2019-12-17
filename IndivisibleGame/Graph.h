#pragma once
#include "Board.h"
#include "Node.h"
#include "Grouping.h"
#include "Results.h"
#include <array>
#include <memory>
#include <vector>
#define SIZE 5 // todo

class Graph {
public:
	Graph();
	std::vector<std::vector<Node const*>> get_selections() const;
	void input_selection(std::vector<Coordinate> const& coords);
	void undo_grouping();
	void clear_groupings();
	Node const* get_node(Coordinate const& coord) const;
	Node const* get_node(int const& x, int const& y) const;
	std::vector<std::weak_ptr<Grouping const>> get_groupings() const;
	std::weak_ptr<Grouping const> get_last_grouping() const;
	bool is_complete() const;
	Results get_popular_results() const;
	Results get_results() const;
	Party const* get_party_a() const;
	Party const* get_party_b() const;
	void print() const;  // debug

private:
	std::array<std::array<Node, SIZE>, SIZE> nodes;
	std::vector<std::shared_ptr<Grouping>> groupings;

	Party m_party_a;
	Party m_party_b;

	void set_adjacency_list(Node& n);
	void dfs_selections(Node const* node, Board<bool>& visited, std::vector<Node const*>& connected) const;
	std::vector<std::string> print_node(Node const& n) const;
	void print_selection_results(bool const& success);
};