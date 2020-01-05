#pragma once
#include <vector>

enum class EventType {
	None = 0,
	GroupingSucceeded, GroupingFailed,
	GroupingUndoSucceeded, GroupingUndoFailed,
	GroupingClearSucceeded, GroupingClearFailed,
	TieBroken, GroupingInputError,
	// InputErrOutOfBounds, InputErrAlreadySelected, InputErrPocket, InputErrSize,
};

class Event {
public:
	bool handled = false;

	virtual EventType get_event_type() const = 0;
};

class IEventListener {
public:
	virtual void on_event(Event& e) = 0;
};

class EventRegistry {
public:
	void process_event(Event& e) {
		for (auto listener : m_listeners) {
			listener->on_event(e);
		}
	}

	void process_event(Event& e) const {
		for (auto listener : m_listeners) {
			listener->on_event(e);
		}
	}

	std::vector<IEventListener*> m_listeners;
};

/*
class EventDispatcher {
public:
	EventDispatcher(Event& event)
		: m_event(event) {}

	// F will be deduced by the compiler
	template<typename T, typename F>
	bool dispatch(const F& func)
	{
		if (m_event.GetEventType() == T::GetStaticType())
		{
			m_event.handled = func(static_cast<T&>(m_event));
			return true;
		}
		return false;
	}
private:
	Event& m_event;
};
*/
