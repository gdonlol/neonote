/**
 * @file Calendar.h
 * @brief Defines the Calendar class for managing and displaying events.
 * 

 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Event.h"
#include <vector>
#include <string>
#include <ncurses.h>

/**
 * @class Calendar
 * @brief Manages a collection of events and provides functionality for rendering and interacting with a calendar.
 * 
 * The Calendar class allows adding, removing, and updating events. It also handles rendering the calendar
 * in an ncurses window, provides date-related utilities, and manages the selection of events.
 * 
 * @author Gordon Xu
 * @author Emily Atyeo
 * @author Edward Bulmer
 */
class Calendar {
public:
    Calendar(WINDOW *content);
    void addEvent(const Event& event);
    void removeEvent(int index);
    void updateEvent(int eventId, Event& updatedEvent);
    std::vector<Event> getEvents();

    int getCurrentDay() const;
    int getCurrentMonth() const;
    int getCurrentYear() const;
    int getDaysInMonth(int month, int year) const;
    int getFirstDayOfMonth(int month, int year) const;

    void renderCalendar();
    int nextFree();

    void setSelectedEvent(int index);
    int getSelectedEvent();
    
private:
    std::vector<Event> events;
    WINDOW *content;

    int selectedEvent;
    int eventsScrollOffset;
};

#endif // CALENDAR_H
