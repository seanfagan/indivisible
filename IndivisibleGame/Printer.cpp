#include "Grouping.h"
#include "Node.h"
#include "Party.h"
#include "Printer.h"
#include "Results.h"
#include <iostream>

Printer::Printer() {};

void Printer::print(Graph& g) const {
	auto nodes = g.get_nodes();

	// top line
	std::cout << "   ";
	for (int i = 0; i < nodes.size(); ++i) {
		std::cout << "+-----";
	}
	std::cout << "+" << std::endl;

	// iterate through node rows in reverse (max to min)
	for (int row = nodes.size() - 1; row >= 0; --row) {
		// construct this row's lines of text, node by node
		std::vector<std::string> row_lines(4);
		for (int col = 0; col < nodes.size(); ++col) {
			const Node* n = nodes[row][col];
			std::vector<std::string> node_lines = print_node(n);
			for (int i = 0; i < node_lines.size(); ++i) {
				row_lines[i] += node_lines[i];
			}
		}

		// print row, line by line
		for (int i = 0; i < row_lines.size(); ++i) {
			// print prefix
			switch (i) {
			case 1:
				std::cout << std::to_string(row + 1) + "  |";
				break;
			case 3:
				std::cout << "   +";
				break;
			default: std::cout << "   |";
			}
			std::cout << row_lines[i] << std::endl;  // print line
		}
	}

	// bottom line
	std::cout << "    ";
	for (int i = 0; i < nodes.size(); i++) {
		char index = 'A' + i;
		std::cout << "  " << std::string{ index } +"   ";
	}
	std::cout << std::endl;
}

void Printer::on_event(Event& e) {
	EventType t = e.get_event_type();

	switch (t) {
	case EventType::None:
		break;
	case EventType::GroupingSucceeded:
		on_grouping_succeeded(static_cast<GroupingSucceededEvent&>(e));
		break;
	case EventType::GroupingFailed:
		on_grouping_failed(static_cast<GroupingFailedEvent&>(e));
		break;
	case EventType::GroupingUndoSucceeded:
		on_grouping_undo_succeeded(static_cast<GroupingUndoSucceededEvent&>(e));
		break;
	case EventType::GroupingUndoFailed:
		on_grouping_undo_failed(static_cast<GroupingUndoFailedEvent&>(e));
		break;
	case EventType::GroupingClearSucceeded:
		on_grouping_clear_succeeded(static_cast<GroupingClearSucceededEvent&>(e));
		break;
	case EventType::GroupingClearFailed:
		on_grouping_clear_failed(static_cast<GroupingClearFailedEvent&>(e));
		break;
	case EventType::TieBroken:
		on_tie_broken(static_cast<TieBrokenEvent&>(e));
		break;
	case EventType::GroupingInputError:
		on_grouping_input_error(static_cast<GroupingInputErrorEvent&>(e));
		break;
	default:
		break;
	}
}

std::vector<std::string> Printer::print_node(const Node* n) const {
	// todo: comment and document. remove magic numbers?
	std::string middle_ln;
	std::string filler_ln;
	{
		if (n->m_grouping == NULL) {
			middle_ln = "  " + std::to_string(n->m_population) + "  ";
			filler_ln = " " + std::string(3, n->m_party->m_ascii) + " ";
		}
		else {
			const Party* w = n->m_grouping->get_results().get_winner();
			if (w) {
				middle_ln = " " + std::string(3, w->m_ascii) + " ";
			}
			else {
				// tie
				middle_ln = " " + std::string(3, '?') + " ";
			}
			filler_ln = std::string(5, ' ');
		}
		// add suffix
		bool connected = false;
		for (auto& neigh : n->m_adjacents) {
			// find eastern neighbor
			if (neigh->m_coord.x > n->m_coord.x) {
				connected = (neigh->m_grouping == n->m_grouping);
			}
		}
		middle_ln += (connected ? " " : "|");
		filler_ln += (connected ? " " : "|");
	}
	std::string bottom_ln;
	{
		// print the separators
		bool connected = false;
		for (auto& neigh : n->m_adjacents) {
			// find vertical neighbor
			if (neigh->m_coord.y < n->m_coord.y) {
				connected = (neigh->m_grouping == n->m_grouping);
			}
		}
		bottom_ln = (connected ? "     " : "-----");
		bottom_ln += "+";
	}
	std::vector<std::string> lines{ filler_ln, middle_ln, filler_ln, bottom_ln };
	return lines;
}

bool Printer::on_grouping_succeeded(GroupingSucceededEvent& e) const {
	std::weak_ptr<Grouping> weak_group = e.get_grouping();
	if (std::shared_ptr<Grouping> group = weak_group.lock()) {
		Results r = group->get_results();
		Party const* winner = r.get_winner();

		if (winner) {
			std::cout << "[!] District created! " << winner->m_name_plural << " win, "
				<< r.get_result(winner) << " out of " << r.get_total() << " votes." << std::endl;
		}
		else {
			std::cout << "[!] Swing district created! Polling is tied." << std::endl;
		}
	}
	else {
		// todo: Shared pointer expired
		std::cout << "This shouldn't happen." << std::endl;
	}
	return true;
}

bool Printer::on_grouping_failed(GroupingFailedEvent& e) const {
	std::cout << "[x] Your selection was not applied." << std::endl;
	return true;
}

bool Printer::on_grouping_undo_succeeded(GroupingUndoSucceededEvent& e) const {
	std::cout << "[i] Last grouping undone." << std::endl;
	return true;
}

bool Printer::on_grouping_undo_failed(GroupingUndoFailedEvent& e) const {
	std::cout << "[i] Nothing to undo my friend." << std::endl;
	return true;
}

bool Printer::on_grouping_clear_succeeded(GroupingClearSucceededEvent& e) const {
	std::cout << "[i] All groupings cleared." << std::endl;
	return true;
}

bool Printer::on_grouping_clear_failed(GroupingClearFailedEvent& e) const {
	std::cout << "[i] Nothing to reset." << std::endl;
	return true;
}

bool Printer::on_tie_broken(TieBrokenEvent& e) const {
	auto tie_winner = e.get_winner();
	std::cout << "[i] Swing district has swung to " << tie_winner->m_name_plural << "!" << std::endl;
	return true;
}

bool Printer::on_grouping_input_error(GroupingInputErrorEvent& e) const {
	std::string reason = e.get_reason();
	std::cout << "[x] Illegal! " << reason << std::endl;
	return true;
}