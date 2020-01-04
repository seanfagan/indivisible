#pragma once
#include "Board.h"
#include "Grouping.h"
#include "Node.h"
#include "Results.h"
#include "Event.h"
#include "Events.h"

#include <array>
#include <memory>
#include <vector>
#include <functional>
#define SIZE 5 // todo

class Graph {
public:
	Graph();
	std::vector<std::vector<Node const*>> get_selections() const;
	void input_selection(std::vector<Coordinate> const& coords);
	void undo_grouping();
	void clear_groupings();
	std::array<std::array<Node const*, SIZE>, SIZE> get_nodes() const;
	Node const* get_node(Coordinate const& coord) const;
	Node const* get_node(int const& x, int const& y) const;
	std::vector<std::weak_ptr<Grouping const>> get_groupings() const;
	std::weak_ptr<Grouping const> get_last_grouping() const;
	bool is_complete() const;
	Results get_popular_results() const;
	Results get_results() const;
	Party const* get_party_a() const;
	Party const* get_party_b() const;

	EventRegistry m_event_registry;

private:
	std::array<std::array<Node, SIZE>, SIZE> nodes;
	std::vector<std::shared_ptr<Grouping>> groupings;

	Party m_party_a;
	Party m_party_b;

	void set_adjacency_list(Node& n);
	void dfs_selections(Node const* node, Board<bool>& visited, std::vector<Node const*>& connected) const;
};