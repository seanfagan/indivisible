#include "Coordinate.h"
#include <tuple>

// static
std::tuple<int, int> Coordinate::to_cartesian(const std::string& notation) {
	/** Convert coordinates from board notation ("A1") to cartesian (0, 0). */
	int a = notation[0] - 'A';
	int b = notation[1] - '1';

	return std::tuple<int, int>(a, b);
}

std::string Coordinate::to_notation(const int& x, const int& y) {
	/** Convert coordinates from cartesian (0, 0) to board notation ("A1"). */
	char a = 'A' + x;
	char b = '1' + y;

	return std::string{ a, b };
}

// nonstatic
Coordinate::Coordinate()
	: x(0), y(0) {}

Coordinate::Coordinate(const int& x, const int& y)
	: x(x), y(y) {}

Coordinate::Coordinate(const std::string& notation) {
	std::tuple<int, int> xy = to_cartesian(notation);
	x = std::get<0>(xy);
	y = std::get<1>(xy);
}

