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

        int getCurrentDay() const;
        int getCurrentMonth() const;
        int getCurrentYear() const;
        int getDaysInMonth(int month, int year) const;
        int getFirstDayOfMonth(int month, int year) const;

        void renderCalendar();
    
private:
    std::vector<Event> events;
    WINDOW *content;
};

#endif // CALENDAR_H
