#include "Calendar.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include "Event.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

/**
 * @brief Constructor for the Calendar class.
 * @param content Pointer to the ncurses window where the calendar will be rendered.
 */
Calendar::Calendar(WINDOW *content) {
    this->content = content;
    std::string path = getenv("HOME");
    path += "/.local/share/neonote/events";

    // Check if the directory exists, if not, create it
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path);
    }

    // Open the directory to read files
    DIR *dir = opendir(path.c_str());
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) { //**< Ensure it's a regular file */
                std::string file_path = path + "/" + entry->d_name;
                std::ifstream file(file_path);
                if (file.is_open()) {
                    std::string line1, line2, line3, line4;
                    std::getline(file, line1);
                    std::getline(file, line2);
                    std::getline(file, line3);
                    std::getline(file, line4);
                    Event event(1, line1, line2, line3);
                    events.push_back(event);
                    file.close();
                }
            }
        }
        closedir(dir);
    }
}

/**
 * @brief Adds an event to the calendar.
 * @param event The event to add.
 */
void Calendar::addEvent(const Event& event) {
    // Get the directory path
    std::string path = getenv("HOME");
    path += "/.local/share/neonote/events";

    // Create a file path using the event's ID
    std::string file_path = path + "/" + std::to_string(event.getId());

    // Open the file for writing
    std::ofstream file(file_path);
    if (file.is_open()) {
        // Write each parameter of the event to the file
        file << event.getTitle() << std::endl;
        file << event.getDate() << std::endl;
        file << event.getDescription() << std::endl;
        file.close();
    }

    // Add the event to the events vector
    events.push_back(event);
}

/**
 * @brief Removes an event from the calendar by its ID.
 * @param eventId The ID of the event to remove.
 */
void Calendar::removeEvent(int eventId) {
    auto it = std::remove_if(events.begin(), events.end(), 
                             [eventId](const Event& event) { return event.getId() == eventId; });

    if (it != events.end()) {
        events.erase(it, events.end());
    }
}

/**
 * @brief Updates an existing event in the calendar.
 * @param eventId The ID of the event to update.
 * @param updatedEvent The new event data.
 */
void Calendar::updateEvent(int eventId, Event& updatedEvent) {
    for (auto& event : events) {
        if (event.getId() == eventId) {
            event = updatedEvent;
            return;
        }
    }
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
    const int dayWidth = (((COLS - 1) * 0.75) / 2) / 7;  //**< Width of each day window
    const int dayHeight = (LINES * 0.75) / 7;                 //**< Height of each day window

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
        mvwprintw(dayWin, 1, 2, "%2d", day);  //**< Display day number

        //Highlight the current day
        if (day == currentDay) {
            wattron(dayWin, A_REVERSE);
            mvwprintw(dayWin, 1, 2, "%2d", day);
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

    WINDOW *eventswin = derwin(
        content, 
        LINES - 4,
        ((COLS * 0.75) / 2 )- 4,
        2,
        ((COLS * 0.75) / 2) + 2
    );
    refresh();
    mvwprintw(eventswin, 0, 0, "%s", "Events");
    wrefresh(eventswin);
    int y = 1;
    for (const Event& event : events) {
        // Print the event details (id, title, description, date, etc.)
        mvwhline(eventswin, y++, 0, ACS_HLINE, ((COLS * 0.75) / 2 )- 4);
        mvwprintw(eventswin, y++, 0, "Event ID: %d", event.getId());
        mvwprintw(eventswin, y++, 0, "Title: %s", event.getTitle().c_str());
        mvwprintw(eventswin, y++, 0, "Description: %s", event.getDescription().c_str());
        mvwprintw(eventswin, y++, 0, "Date: %s", event.getDate().c_str());
    }
    //Refresh the main content window
    wrefresh(content);

    //Cleanup: Free memory by deleting all sub-windows.
    for (auto win : dayWindows) {
        delwin(win);
    }
}