#pragma once
#include <unordered_map>
#include "Party.h"

class Results {
public:
	Results(const std::unordered_map<Party const*, int>& results);
	Party const* get_winner() const;
	int get_total() const;
	int get_result(Party const* party) const;
private:
	std::unordered_map<Party const*, int> m_results;
	Party const* m_winner;
	int m_total;
};