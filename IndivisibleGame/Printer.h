#pragma once
#include "Event.h"
#include "Events.h"
#include "Graph.h"
#include <functional>

class Printer : public IEventListener {
public:
	// Constructors
	Printer();

	// Methods
	void print(Graph& g) const;
	virtual void on_event(Event& e) override;

private:
	std::vector<std::string> print_node(const Node* n) const;

	bool on_grouping_succeeded(GroupingSucceededEvent& e) const;
	bool on_grouping_failed(GroupingFailedEvent& e) const;
	bool on_grouping_undo_succeeded(GroupingUndoSucceededEvent& e) const;
	bool on_grouping_undo_failed(GroupingUndoFailedEvent& e) const;
	bool on_grouping_clear_succeeded(GroupingClearSucceededEvent& e) const;
	bool on_grouping_clear_failed(GroupingClearFailedEvent& e) const;
	bool on_tie_broken(TieBrokenEvent& e) const;
};