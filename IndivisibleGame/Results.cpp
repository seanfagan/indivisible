#include "Results.h"

// Constructors
Results::Results(std::unordered_map<Party const*, int> const& results)
	: m_results(results) {
	/** Use results to calculate total and winner. */
	int max = 0;

	for (std::pair<Party const*, int> r : m_results) {
		// Tally total
		m_total += r.second;

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

// Methods
int Results::get_result(Party const* party) const
{
	return m_results.at(party);
}

int Results::get_total() const
{
	return m_total;
}

Party const* Results::get_winner() const
{
	return m_winner;
}
