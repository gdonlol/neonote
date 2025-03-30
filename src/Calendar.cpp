#include "Calendar.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <ncurses.h>

using namespace std;

/**
 * @brief Constructor for the Calendar class.
 * @param content Pointer to the ncurses window where the calendar will be rendered.
 */
Calendar::Calendar(WINDOW *content) {
    this->content = content;
}

/**
 * @brief Adds an event to the calendar.
 * @param event The event to add.
 */
void Calendar::addEvent(const Event& event) {
    events.push_back(event);
}

/**
 * @brief Gets the current day of the month.
 * @return The current day (1-31).
 */
int Calendar::getCurrentDay() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mday;
}

/**
 * @brief Gets the current month.
 * @return The current month as an integer (1 = January, 12 = December).
 */
int Calendar::getCurrentMonth() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mon + 1;
}

/**
 * @brief Gets the current year.
 * @return The current year (e.g., 2025).
 */
int Calendar::getCurrentYear() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_year + 1900;
}

/**
 * @brief Gets the number of days in a specified month and year.
 * @param month The month (1-12).
 * @param year The year (e.g., 2025).
 * @return The number of days in the month.
 */
int Calendar::getDaysInMonth(int month, int year) const {
    if (month == 2) {
        // Handle leap years
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        }
        return 28;
    }

    // April, June, September, November have 30 days
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }

    // All other months have 31 days
    return 31;
}

/**
 * @brief Gets the starting day of the specified month and year.
 * @param month The month (1-12).
 * @param year The year (e.g., 2025).
 * @return The starting day of the month (0 = Sunday, 6 = Saturday).
 */
int Calendar::getFirstDayOfMonth(int month, int year) const {
    tm time_in = {};
    time_in.tm_year = year - 1900;     //**< Year since 1900 */
    time_in.tm_mon = month - 1;        //**< Month (0-11) */
    time_in.tm_mday = 1;               //**< First day of the month */

    mktime(&time_in);
    return time_in.tm_wday;            //**< 0 = Sunday, 6 = Saturday */
}

/**
 * @brief Renders the calendar for the current month.
 * 
 * This function creates a grid of sub-windows inside the `content` window to represent 
 * each day of the month. It displays the current month and year, highlights the current day, 
 * and organizes the days in a weekly format.
 */
void Calendar::renderCalendar() {
    wclear(content);
    box(content, 0, 0);

    //Display Header: Shows the current month, year, and weekdays.
    int currentMonth = getCurrentMonth();
    int year = getCurrentYear();
    mvwprintw(content, 2, 2, "%d/%d", currentMonth, year);  //**< Display "MM/YYYY"

    //Month Details: Calculates the number of days and the first weekday.
    int daysInMonth = getDaysInMonth(currentMonth, year);
    int firstDay = getFirstDayOfMonth(currentMonth, year);

    //Grid Layout: Defines dimensions of each day sub-window.
    const int dayWidth = ((COLS - 1) * 0.75) / 7;  //**< Width of each day window
    const int dayHeight = 4;                 //**< Height of each day window

    int startY = 6;                          //**< Starting Y position for the first row
    int startX = 2 + (firstDay * dayWidth);  //**< Starting X position based on the first weekday

    //Draw headers
    const char* weekdays[] = {"S", "M", "T", "W", "T", "F", "S"};
    int headerY = 4;
    int headerX = 2;
    for (int i = 0; i < 7; ++i) {
        mvwprintw(content, headerY, headerX + (i * dayWidth) + (dayWidth / 2) - 1, "%s", weekdays[i]);
    }

    int currentDay = getCurrentDay();        //**< Get the current day for highlighting

    std::vector<WINDOW*> dayWindows;         //**< Store sub-windows for cleanup

    //Render Grid: Creates sub-windows and displays day numbers.
    for (int day = 1; day <= daysInMonth; ++day) {
        WINDOW* dayWin = derwin(content, dayHeight, dayWidth, startY, startX);  //**< Create day sub-window
        dayWindows.push_back(dayWin);  //**< Store the window for later cleanup

        box(dayWin, 0, 0);  //**< Draw the border
        mvwprintw(dayWin, 2, 2, "%2d", day);  //**< Display day number

        //Highlight the current day
        if (day == currentDay) {
            wattron(dayWin, A_REVERSE);
            mvwprintw(dayWin, 2, 2, "%2d", day);
            wattroff(dayWin, A_REVERSE);
        }

        wrefresh(dayWin);

        //Move to the next day position
        startX += dayWidth;

        //Move to the next row at the end of the week
        if ((firstDay + day) % 7 == 0) {
            startY += dayHeight;
            startX = 2;  //**< Reset to the first column
        }
    }

    //Refresh the main content window
    wrefresh(content);

    //Cleanup: Free memory by deleting all sub-windows.
    for (auto win : dayWindows) {
        delwin(win);
    }
}