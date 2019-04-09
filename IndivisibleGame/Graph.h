#pragma once
#include "Component.h"
#include "Board.h"
#include <vector>
#include <array>
#define SIZE 5

class Graph {
public:
	Graph(const int& seed);
	std::vector<Component> get_selections();
private:
	struct Node {
		enum Party : char {
			A, B
		};

		Coordinate m_coord;
		Party m_party;
		int m_population = 0;
		std::vector<Coordinate> m_adjacents;
		int m_selection = 0;
	};

	std::array<std::array<Node, SIZE>, SIZE> nodes;

	void initialize(const int& seed);
	void set_adjacency_list(Node& n);
	int dfs_selections(const Coordinate& coord, Board<bool>& visited, int size);
};