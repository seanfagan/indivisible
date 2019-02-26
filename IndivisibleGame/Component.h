#pragma once
#include "Coordinate.h"

struct Component {
	Coordinate root;
	int size;
	int group;

	Component(const Coordinate& root, const int& size, const int& group);
};
