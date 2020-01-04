#pragma once

#include "Event.h"

class Grouping;

class GroupingSucceededEvent : public Event {
public:
	GroupingSucceededEvent(std::weak_ptr<Grouping> grouping)
		: m_grouping(grouping) {}

	inline std::weak_ptr<Grouping> get_grouping() const { return m_grouping;}

	virtual EventType get_event_type() const override {
		return EventType::GroupingSucceeded;
	}
private:
	std::weak_ptr<Grouping> m_grouping;
};

class GroupingFailedEvent : public Event {
public:
	GroupingFailedEvent() {}

	virtual EventType get_event_type() const override {
		return EventType::GroupingFailed;
	}
};

class GroupingUndoSucceededEvent : public Event {
public:
	GroupingUndoSucceededEvent() {}

	virtual EventType get_event_type() const override {
		return EventType::GroupingUndoSucceeded;
	}
};

class GroupingUndoFailedEvent : public Event {
public:
	GroupingUndoFailedEvent() {}

	virtual EventType get_event_type() const override {
		return EventType::GroupingUndoFailed;
	}
};

class GroupingClearSucceededEvent : public Event {
public:
	GroupingClearSucceededEvent() {}

	virtual EventType get_event_type() const override {
		return EventType::GroupingClearSucceeded;
	}
};

class GroupingClearFailedEvent : public Event {
public:
	GroupingClearFailedEvent() {}

	virtual EventType get_event_type() const override {
		return EventType::GroupingClearFailed;
	}
};

class TieBrokenEvent : public Event {
public:
	TieBrokenEvent(Party const* winner)
		: m_winner(winner) {}

	inline Party const* get_winner() const { return m_winner; }

	virtual EventType get_event_type() const override {
		return EventType::TieBroken;
	}
private:
	Party const* m_winner;
};
