#include "Coordinate.h"

// Constructors
Coordinate::Coordinate()
	: x(0), y(0) {}

Coordinate::Coordinate(int const& x, int const& y)
	: x(x), y(y) {}

Coordinate::Coordinate(std::string const& notation) {
	std::tuple<int, int> xy = to_cartesian(notation);
	x = std::get<0>(xy);
	y = std::get<1>(xy);
}

// Operators
std::ostream& operator<<(std::ostream& os, Coordinate& d)
{
	os << d.to_notation(d.x, d.y);
	return os;
}

std::ostream& operator<<(std::ostream& os, Coordinate const& d)
{
	os << d.to_notation(d.x, d.y);
	return os;
}

// Static methods
std::tuple<int, int> Coordinate::to_cartesian(std::string const& notation) {
	/** Convert coordinates from board notation ("A1") to cartesian (0, 0). */
	int a = notation[0] - 'A';
	int b = notation[1] - '1';

	return std::tuple<int, int>(a, b);
}

std::string Coordinate::to_notation(int const& x, int const& y) {
	/** Convert coordinates from cartesian (0, 0) to board notation ("A1"). */
	char a = 'A' + x;
	char b = '1' + y;

	return std::string{ a, b };
}
