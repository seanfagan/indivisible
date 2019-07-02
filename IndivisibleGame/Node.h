#pragma once
#include "Coordinate.h"
#include <vector>

class Grouping;  // forward declaration

struct Node {
	enum Party : char {
		A, B
	};

	Coordinate m_coord;
	Party m_party;
	int m_population = 0;
	std::vector<const Node*> m_adjacents;
	const Grouping* m_grouping = NULL;
};