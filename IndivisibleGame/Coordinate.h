#pragma once
#include <string>
#include <tuple>


struct Coordinate {
	// Vars
	int x, y;

	// Constructors
	Coordinate();
	Coordinate(int const& x, int const& y);
	Coordinate(std::string const& notation);

	// Operators
	friend std::ostream& operator<<(std::ostream& os, Coordinate& d);
	friend std::ostream& operator<<(std::ostream& os, Coordinate const& d);

	// Static methods
	static std::tuple<int, int> to_cartesian(std::string const& notation);
	static std::string to_notation(int const& x, int const& y);
};