#ifndef CALENDAR_H
#define CALENDAR_H

#include "Event.h"
#include <vector>
#include <string>
#include <ncurses.h>

class Calendar {
public:
        Calendar(WINDOW *content);
        void addEvent(const Event& event);
        void removeEvent(int eventId);
        void updateEvent(int eventId, Event& updatedEvent);

        int getCurrentDay() const;
        int getCurrentMonth() const;
        int getCurrentYear() const;
        int getDaysInMonth(int month, int year) const;
        int getFirstDayOfMonth(int month, int year) const;

        void renderCalendar();
        int nextFree();
    
private:
    std::vector<Event> events;
    WINDOW *content;
};

#endif // CALENDAR_H
