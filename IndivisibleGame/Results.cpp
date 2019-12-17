#include "Results.h"

Results::Results(const std::unordered_map<Party const*, int>& results)
	: m_results(results) {
	// Use results to calculate total and winner.
	int max = 0;
	for (std::pair<Party const*, int> r : m_results) {
		m_total += r.second;  // tally total
		// Is this the winner?
		if (r.second > max) {
			m_winner = r.first;  // new winner
			max = r.second;
		}
		else if (r.second == max) {
			m_winner = 0;  // tie, or no result
		}
	}
}

Party const* Results::get_winner() const
{
	return m_winner;
}

int Results::get_total() const
{
	return m_total;
}

int Results::get_result(Party const* party) const
{
	int hella = 1;
	hella = m_results.at(party);
	hella = 999;
	return m_results.at(party);
}
