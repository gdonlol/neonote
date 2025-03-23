#include "Calendar.h"

Calendar::Calendar() {}

void Calendar::addEvent(const Event& event) {
    events.push_back(event);
}

void Calendar::removeEvent(int eventId) {
    for(auto it = events.begin(); it != events.end(); ++it) {
        if(it->getId() == eventId) {
            events.erase(it);
            break;
        }
    }
}

std::vector<Event> Calendar::getEvents() const {
    return events;
}

void Calendar::updateEvent(int eventId, const Event& updatedEvent) {
    for(auto & event : events) {
        if(event.getId() == eventId) {
            event = updatedEvent;
            break;
        }
    }
}

void Calendar::renderCalendar() {
    // Stub: Rendering should be handled by the TUI.
}
