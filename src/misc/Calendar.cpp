#include "Calendar.h"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

void Calendar::addEvent(const Event& event) {
    events.push_back(event);
    std::cout << "Event added: " << event.toString() << std::endl;
}

void Calendar::printEvents() const {
    std::cout << "\n--- Calendar Events ---\n";
    for (const auto& event : events) {
        std::cout << event.toString() << std::endl;
    }
    std::cout << "-----------------------\n";
}

std::vector<Event> Calendar::getEvents() const {
    return events;
}

// get current day of the month
int Calendar::getCurrentDay() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mday;
}

// get current month (1 = January, 12 = December)
int Calendar::getCurrentMonth() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mon + 1;
}

// get current year
int Calendar::getCurrentYear() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_year + 1900;
}

// get number of days in the specified month and year
int Calendar::getDaysInMonth(int month, int year) const {
    if (month == 2) {
        // handle leap years
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        }
        return 28;
    }

    // april, june, september, november have 30 days
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }

    // all other months have 31 days
    return 31;
}

// get the starting day of the month (0 = sunday, 6 = saturday)
int Calendar::getFirstDayOfMonth(int month, int year) const {
    tm time_in = {};
    time_in.tm_year = year - 1900; // year since 1900
    time_in.tm_mon = month - 1;    // month (0-11)
    time_in.tm_mday = 1;           // first day of the month

    mktime(&time_in);
    return time_in.tm_wday; // 0 = sunday, 6 = saturday
}
