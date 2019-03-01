#pragma once
#include <string>

struct Coordinate {
	int x, y;

	Coordinate();
	Coordinate(const int& x, const int& y);
	Coordinate(const std::string& notation);

	static std::tuple<int, int> to_cartesian(const std::string& notation);
	static std::string to_notation(const int& x, const int& y);
};