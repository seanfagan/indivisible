#pragma once
#include "Coordinate.h"
#include <vector>

class Grouping;  // forward declaration
class Party; // forward declaration

struct Node {
	Coordinate m_coord;
	Party const* m_party;
	int m_population = 0;
	std::vector<Node const*> m_adjacents;
	Grouping const* m_grouping = NULL;
};