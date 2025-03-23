#ifndef CALENDAR_H
#define CALENDAR_H

#include "Event.h"
#include <vector>

class Calendar {
public:
    void addEvent(const Event& event);
    void printEvents() const;
    
private:
    std::vector<Event> events;
};

#endif // CALENDAR_H
