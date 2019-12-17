#pragma once
#include <unordered_map>
#include "Party.h"

class Results {
public:
	// Constructors
	Results(std::unordered_map<Party const*, int> const& results);

	// Methods
	int get_result(Party const* party) const;
	int get_total() const;
	Party const* get_winner() const;

private:
	// Vars
	std::unordered_map<Party const*, int> m_results;
	int m_total;
	Party const* m_winner;
};