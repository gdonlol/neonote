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
 * @brief Renders the calendar for the current month and year.
 *        Displays the month, year, and highlights the current day.
 */
void Calendar::renderCalendar() {
    wclear(content);
    box(content, 0, 0);

    // Display the month and year
    int currentMonth = getCurrentMonth();
    int year = getCurrentYear();
    mvwprintw(content, 2, 2, "%d/%d", currentMonth, year);  //**< Display as MM/YYYY */
    mvwprintw(content, 3, 2, "S  M  T  W  T  F  S");

    // Get days in the current month and the first day
    int daysInMonth = getDaysInMonth(currentMonth, year);
    int firstDay = getFirstDayOfMonth(currentMonth, year);

    // Print the calendar days
    int row = 4;
    int col = 2 + (firstDay * 3);

    for (int day = 1; day <= daysInMonth; ++day) {
        mvwprintw(content, row, col, "%2d", day);

        col += 3;  //**< Move to the next column */

        // Move to the next row at the end of the week
        if ((firstDay + day) % 7 == 0) {
            row++;
            col = 2;  //**< Reset to the first column of the week */
        }
    }

    // Highlight the current day
    int currentDay = getCurrentDay();

    // Calculate the position of the current day
    row = 4 + (firstDay + currentDay - 1) / 7;
    col = 2 + ((firstDay + currentDay - 1) % 7) * 3;

    // Ensure the current day is within the valid range before highlighting
    if (currentDay <= daysInMonth) {
        wattron(content, A_REVERSE);  //**< Invert colors for the current day */
        mvwprintw(content, row, col, "%2d", currentDay);
        wattroff(content, A_REVERSE);
    }

    wrefresh(content);
}
