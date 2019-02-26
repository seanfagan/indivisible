#pragma once
#include "Coordinate.h"
#include <array>

template<typename T>
class Board {
public:
	Board(const T& init_value) {
		fill(init_value);
	}
	Board(const std::array<std::array<T, 5>, 5>& init_array) {
		m_arr = init_array;
	}

	T get(const Coordinate& coord) const {
		return m_arr[coord.row][coord.col];
	}

	T get(const int& x, const int& y) const {
		return m_arr[y][x];
	}

	void set(const Coordinate& coord, const T& value) {
		m_arr[coord.row][coord.col] = value;
	}

	void set(const int& x, const int& y, const T& value) {
		m_arr[y][x] = value;
	}

	void fill(const T& value) {
		for (int y = 0; y < m_arr.size(); ++y) {
			for (int x = 0; x < m_arr[0].size(); ++x) {
				set(x, y, value);
			}
		}
	}

private:
	std::array<std::array<T, 5>, 5> m_arr; // todo: size
};