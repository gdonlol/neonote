#ifndef CALENDAR_H
#define CALENDAR_H

#include "Event.h"
#include <vector>
#include <string>

class Calendar {
private:
    std::vector<Event> events;
public:
    Calendar();
    void addEvent(const Event& event);
    void removeEvent(int eventId);
    std::vector<Event> getEvents() const;
    void updateEvent(int eventId, const Event& updatedEvent);
    void renderCalendar(); // Stub for rendering calendar in the TUI
};

#endif // CALENDAR_H
