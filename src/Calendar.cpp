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
 * @class Calendar
 * @brief Represents a calendar to manage events.
 * 
 * Handles event rendering, loading from disk, and interactions.
 * 
 * @author Gordon Xu
 */
Calendar::Calendar(WINDOW *content): selectedEvent(-1) {
    this->content = content;
    std::string path = getenv("HOME");
    path += "/.local/share/neonote/events";

    // Create events directory if it doesn't exist
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path);
    }

    // Load events from the directory
    DIR *dir = opendir(path.c_str());
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                std::string file_path = path + "/" + entry->d_name;
                std::ifstream file(file_path);
                if (file.is_open()) {
                    std::string line1, line2, line3, line4;
                    std::getline(file, line1);
                    std::getline(file, line2);
                    std::getline(file, line3);
                    std::getline(file, line4);

                    std::string filename = entry->d_name;
                    int eventId = 0;
                    try {
                        eventId = std::stoi(filename);
                    } catch (const std::invalid_argument& e) {
                        exit(0);  // Invalid file name, exit
                    }

                    Event event(eventId, line1, line2, line3);
                    events.push_back(event);
                    file.close();
                }
            }
        }
        closedir(dir);
    }
}

/**
 * @brief Renders the calendar to the screen.
 */
void Calendar::renderCalendar() {
    wclear(content);  ///< Clear content window
    box(content, 0, 0);  ///< Draw border

    int maxY, maxX;
    getmaxyx(content, maxY, maxX);  ///< Get window size

    int contentMaxY, contentMaxX;
    getmaxyx(content, contentMaxY, contentMaxX);  ///< Get content window size

    // Print current month/year
    int currentMonth = getCurrentMonth();
    int year = getCurrentYear();
    mvwprintw(content, 2, 2, "%d/%d", currentMonth, year);

    int daysInMonth = getDaysInMonth(currentMonth, year);
    int firstDay = getFirstDayOfMonth(currentMonth, year);

    int dayWidth = (contentMaxX / 2) / 7;
    int dayHeight = (contentMaxY * 0.75) / 7;

    int startY = 6;
    int startX = 2 + (firstDay * dayWidth);

    const char* weekdays[] = {"S", "M", "T", "W", "T", "F", "S"};
    int headerY = 4;
    int headerX = 2;

    // Print weekday headers
    for (int i = 0; i < 7; ++i) {
        mvwprintw(content, headerY, headerX + (i * dayWidth) + (dayWidth / 2) - 1, "%s", weekdays[i]);
    }

    int currentDay = getCurrentDay();
    std::vector<WINDOW*> dayWindows;

    // Draw calendar days
    for (int day = 1; day <= daysInMonth; ++day) {
        WINDOW* dayWin = derwin(content, dayHeight, dayWidth, startY, startX);
        dayWindows.push_back(dayWin);

        box(dayWin, 0, 0);  ///< Draw day border
        mvwprintw(dayWin, 1, 2, "%2d", day);

        // Highlight current day
        if (day == currentDay) {
            wattron(dayWin, A_REVERSE);
            mvwprintw(dayWin, 1, 2, "%2d", day);
            wattroff(dayWin, A_REVERSE);
        }

        wrefresh(dayWin);

        startX += dayWidth;

        // Move to next row if needed
        if ((firstDay + day) % 7 == 0) {
            startY += dayHeight;
            startX = 2;
        }
    }

    // Draw events window
    int eventsWinHeight = contentMaxY - 4;
    int eventsWinWidth = (contentMaxX / 2) - 4;
    int eventsWinY = 2;
    int eventsWinX = (contentMaxX / 2) + 3;

    WINDOW* eventswin = derwin(content, eventsWinHeight, eventsWinWidth, eventsWinY, eventsWinX);
    refresh();
    mvwprintw(eventswin, 0, 0, "%s", "Events");
    wrefresh(eventswin);

    int y = 1;
    int lineWidth = eventsWinWidth - 1;
    int i = 0; 

    // No events message
    if (events.empty()) {
        mvwprintw(eventswin, ++y, 0, "%s", "No events (Ctrl + N)");
    }
    
    // Display events
    for (const Event& event : events) {
        mvwhline(eventswin, y++, 0, ACS_HLINE, lineWidth);

        if (selectedEvent == i) wattron(eventswin, A_REVERSE);

        std::string title = "Title: " + event.getTitle();
        mvwprintw(eventswin, y++, 0, "%s", (title + std::string(lineWidth - title.length(), ' ')).c_str());

        std::string description = "Description: " + event.getDescription();
        mvwprintw(eventswin, y++, 0, "%s", (description + std::string(lineWidth - description.length(), ' ')).c_str());
        
        std::string date = "Date: " + event.getDate();
        mvwprintw(eventswin, y++, 0, "%s", (date + std::string(lineWidth - date.length(), ' ')).c_str());
        
        wattroff(eventswin, A_REVERSE);
        ++i;
    }

    wrefresh(content);

    // Clean up day windows
    for (auto win : dayWindows) {
        delwin(win);
    }
}

/**
 * @brief Adds an event to the calendar.
 * @param event The event to add.
 */
void Calendar::addEvent(const Event& event) {
    std::string path = getenv("HOME");
    path += "/.local/share/neonote/events";

    std::string file_path = path + "/" + std::to_string(event.getId());

    std::ofstream file(file_path);
    if (file.is_open()) {
        file << event.getTitle() << std::endl;
        file << event.getDate() << std::endl;
        file << event.getDescription() << std::endl;
        file.close();
    }

    events.push_back(event);  ///< Add event to list
}

/**
 * @brief Removes an event from the calendar.
 * @param index The index of the event to remove.
 */
void Calendar::removeEvent(int index) {
    if (index < 0 || index >= static_cast<int>(events.size())) {
        std::cerr << "Invalid event index.\n";
        return;
    }
    int eventId = events[index].getId();
    std::string path = getenv("HOME");
    path += "/.local/share/neonote/events/" + std::to_string(eventId);

    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);  ///< Delete event file
    }

    events.erase(events.begin() + index);  ///< Remove event from list
    if (selectedEvent >= static_cast<int>(events.size())) {
        selectedEvent = static_cast<int>(events.size()) - 1;
    }
}

/**
 * @brief Updates an existing event.
 * @param eventId The ID of the event to update.
 * @param updatedEvent The updated event.
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
 * @return The current day.
 */
int Calendar::getCurrentDay() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mday;
}

/**
 * @brief Gets the current month.
 * @return The current month.
 */
int Calendar::getCurrentMonth() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mon + 1;
}

/**
 * @brief Gets the current year.
 * @return The current year.
 */
int Calendar::getCurrentYear() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_year + 1900;
}

/**
 * @brief Gets the number of days in
