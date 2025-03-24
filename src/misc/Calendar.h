#ifndef CALENDAR_H
#define CALENDAR_H

#include "Event.h"
#include <vector>
#include <string>

class Calendar {
public:
        void addEvent(const Event& event);
        void printEvents() const;
    
        // getters
        std::vector<Event> getEvents() const;

        int getCurrentDay() const;
        int getCurrentMonth() const;
        int getCurrentYear() const;
        int getDaysInMonth(int month, int year) const;
        int getFirstDayOfMonth(int month, int year) const;
    
private:
    std::vector<Event> events;
};

#endif // CALENDAR_H
