#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h> 
#include <cstring>
#include <filesystem>
#include <fstream>
#include <cstdlib>

/**
 * @brief Constructor for TaskManager.
 * 
 * Initializes a TaskManager with the given ncurses window where tasks will be displayed.
 * 
 * @param content Pointer to the ncurses window where tasks will be displayed.
 */
TaskManager::TaskManager(WINDOW* content) : content(content), currentSelected(-1), currentType(-1) {
    this->content = content;

    const char* homeDir = getenv("HOME");
    if (!homeDir) return;

    std::string dirPath = std::string(homeDir) + "/.local/share/neonote/kanban/";

    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        std::filesystem::create_directories(dirPath);  // Create the directory if it doesn't exist
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::ifstream inFile(entry.path());
            
            std::string title;
            int type = -1;
            
            if (std::getline(inFile, title) && inFile >> type) {
                std::string filename = entry.path().filename().string();
                int taskId = std::stoi(filename);

                if (type >= 0 && type < tasks.size()) {
                    tasks[type].emplace_back(taskId, title, type);
                }
            }
            inFile.close();
        }
    }
}

/**
 * @brief Adds a task to the task list with the given title.
 * 
 * This function creates a new task with a unique ID, default status ("To Do"), 
 * and the given title. The task is then added to the task list.
 * 
 * @param title The title of the task to be added.
 */
void TaskManager::addTask(const std::string& title, int type) {
    int newTaskId = nextFree();
    tasks[type].emplace_back(newTaskId, title, type);

    const char* homeDir = getenv("HOME");  // Get the home directory
    if (homeDir == nullptr) return;        // Failsafe if HOME is not set

    std::string kanbanDir = std::string(homeDir) + "/.local/share/neonote/kanban/";

    // Create the directory if it doesn't exist
    std::filesystem::create_directories(kanbanDir);

    // Write the task to a file with ID as filename
    std::string filePath = kanbanDir + std::to_string(newTaskId);

    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << title << "\n";
        outFile << type << "\n";
        outFile.close();
    }
}

/**
 * @brief Removes a task from the task list by its ID.
 * 
 * This function searches for a task by its ID and removes it from the task list.
 * 
 * @param taskId The ID of the task to be removed.
 */
void TaskManager::removeTask(int taskId) {
    if (taskId == -1) return;

    for (auto& taskList : tasks) {
        for (auto it = taskList.begin(); it != taskList.end(); ++it) {
            if (it->getId() == taskId) {
                taskList.erase(it);

                const char* homeDir = getenv("HOME");
                if (homeDir != nullptr) {
                    std::string filePath = std::string(homeDir) + "/.local/share/neonote/kanban/" + std::to_string(taskId);

                    if (std::filesystem::exists(filePath)) {
                        std::filesystem::remove(filePath);
                    }
                }
                return;
            }
        }
    }
}

/**
 * @brief Moves a task to a new status.
 * 
 * This function searches for a task by its ID and updates its status to the
 * provided new status.
 * 
 * @param taskId The ID of the task to be moved.
 * @param type The new status to assign to the task.
 */
void TaskManager::moveTask(int taskId, int type) {
    if (type < 0 || type >= tasks.size()) {
        return;
    }

    for (auto& taskList : tasks) {
        for (auto it = taskList.begin(); it != taskList.end();) {  
            if (it->getId() == taskId) {
                if (&taskList != &tasks[type]) {
                    tasks[type].push_back(*it);
                    it = taskList.erase(it);

                    const char* homeDir = getenv("HOME");
                    if (homeDir != nullptr) {
                        std::string filePath = std::string(homeDir) + "/.local/share/neonote/kanban/" + std::to_string(taskId);

                        if (std::filesystem::exists(filePath)) {
                            std::ifstream inFile(filePath);
                            std::string title;

                            if (std::getline(inFile, title)) {
                                inFile.close();

                                std::ofstream outFile(filePath);
                                outFile << title << "\n" << type << "\n";
                                outFile.close();
                            }
                        }
                    }
                } else {
                    ++it;
                }
                return;
            } else {
                ++it;
            }
        }
    }
}

/**
 * @brief Gets a reference to the list of all tasks.
 * @return A reference to the vector containing all tasks.
 */
const std::vector<std::vector<Task>>& TaskManager::getTasks() const {
    return tasks;
}

/**
 * @brief Renders the tasks in the ncurses window.
 * 
 * This function clears the window and displays the list of tasks divided by their
 * status into three columns: "To Do", "In Progress", and "Done".
 */
void TaskManager::renderTasks() {
    werase(content);
    box(content, 0, 0);

    int maxY, maxX;
    getmaxyx(content, maxY, maxX);  // Get the height and width of the window

    // Calculate dimensions for the columns
    int colWidth = (maxX - 2) / 3;        // Divide window into 3 equal parts
    int colHeight = maxY - 2;             // Leave space for borders

    // Create derwin windows for each column
    WINDOW* todoWin = derwin(content, colHeight, colWidth, 1, 1);
    WINDOW* inProgressWin = derwin(content, colHeight, colWidth, 1, colWidth + 1);
    WINDOW* doneWin = derwin(content, colHeight, colWidth, 1, 2 * colWidth + 1);

    // Render headers
    if(currentType == 0) wattron(todoWin, COLOR_PAIR(6));
    mvwprintw(todoWin, 0, (colWidth - 6) / 2, "To Do");
    mvwhline(todoWin, 1, 0, ACS_HLINE, colWidth);
    wattroff(todoWin, COLOR_PAIR(6));

    if(currentType == 1) wattron(inProgressWin, COLOR_PAIR(6));
    mvwprintw(inProgressWin, 0, (colWidth - 11) / 2, "In Progress");
    mvwhline(inProgressWin, 1, 0, ACS_HLINE, colWidth);
    wattroff(inProgressWin, COLOR_PAIR(6));

    if(currentType == 2) wattron(doneWin, COLOR_PAIR(6));
    mvwprintw(doneWin, 0, (colWidth - 4) / 2, "Done");
    mvwhline(doneWin, 1, 0, ACS_HLINE, colWidth);
    wattroff(doneWin, COLOR_PAIR(6));

    // Display tasks in each column
    for (int col = 0; col < 3; ++col) {
        WINDOW* drawWin = (col == 0) ? todoWin : (col == 1) ? inProgressWin : doneWin;

        int maxVisibleEvents = (colHeight / 3) - 1;

        if (currentSelected < colOffset[col]) {
            colOffset[col] = std::max(currentSelected, 0);
        } else if (currentSelected >= colOffset[col] + maxVisibleEvents) {
            colOffset[col] = currentSelected - maxVisibleEvents + 1;
        }
        
        int y = 3;
        for (int i = colOffset[col]; i < std::min(colOffset[col] + maxVisibleEvents, (int)tasks[col].size()); ++i) {
            bool isSelected = (currentType == col && currentSelected == i);

            if (isSelected) {
                wattron(drawWin, A_REVERSE);
            }

            std::string title = " " + tasks[col][i].getTitle() + " ";
            std::string id = " #" + std::to_string(tasks[col][i].getId()) + " ";

            // Elipsis overflow
            if (title.length() > colWidth - 2) {
                title = title.substr(0, colWidth - 5) + "...";
            }

            // Pad both lines to fit the column width
            while (title.length() < colWidth - 2) {
                title += " ";
            }
            while (id.length() < colWidth - 2) {
                id += " ";
            }

            // Draw the task with padding and inverted color if selected
            mvwprintw(drawWin, y++, 1, "%s", title.c_str());
            mvwprintw(drawWin, y++, 1, "%s", id.c_str());
            y++;  // Add spacing between tasks

            if (isSelected) {
                wattroff(drawWin, A_REVERSE);  // Turn off inversion
            }
        }
    }

    // Refresh all windows
    wrefresh(todoWin);
    wrefresh(inProgressWin);
    wrefresh(doneWin);
    wrefresh(content);

    // Clean up windows
    delwin(todoWin);
    delwin(inProgressWin);
    delwin(doneWin);
}

void TaskManager::moveSelection(int direction){
    switch (direction) {
        case 0:  // Up
            if (!tasks[currentType].empty()) {
                currentSelected = (currentSelected - 1 + tasks[currentType].size()) % tasks[currentType].size(); //**< Circular navigation for rows */
            }
            break;

        case 1:  // Down
            if (!tasks[currentType].empty()) {
                currentSelected = (currentSelected + 1) % tasks[currentType].size(); //**< Circular navigation for rows */
            }
            break;

        case 2:  // Left
            currentType = (currentType - 1 + 3) % 3;  //**< Circular navigation for columns */
            currentSelected = std::min(currentSelected, tasks[currentType].size() == 0 ? 0: (int)tasks[currentType].size() - 1);
            break;

        case 3:  // Right
            currentType = (currentType + 1) % 3;  //**< Circular navigation for columns */
            currentSelected = std::min(currentSelected, tasks[currentType].size() == 0 ? 0: (int)tasks[currentType].size() - 1);
            break;
    }
}

int TaskManager::getSelectedTaskId() {
    if (currentType >= tasks.size() || currentSelected >= tasks[currentType].size()) {
        return -1;
    }
    return tasks[currentType][currentSelected].getId();
}

/**
 * @brief Prompts the user to move a task to a different status via a popup.
 * @param taskId The ID of the task to move.
 */
void TaskManager::moveTaskPopup(int taskId) {
    if(taskId == - 1)return;
    const std::vector<std::string> categories = {"To Do", "In Progress", "Done"};
    const std::vector<int> categoriesType = {0, 1, 2};

    int highlight = 0;
    int choice = -1;

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Popup dimensions
    int popupHeight = categories.size() + 4;
    int popupWidth = 20;
    int startY = (maxY - popupHeight) / 2;
    int startX = (maxX - popupWidth) / 2;
    
    // Create popup window
    WINDOW* popup = newwin(popupHeight, popupWidth, startY, startX);
    box(popup, 0, 0);
    keypad(popup, TRUE);

    mvwprintw(popup, 1, 3, "Move Task To:");
    wrefresh(popup);

    while (choice == -1) {
        // Display options
        for (size_t i = 0; i < categories.size(); i++) {
            if (i == highlight) wattron(popup, A_REVERSE);
            mvwprintw(popup, i + 2, 3, "%s", categories[i].c_str());
            wattroff(popup, A_REVERSE);
        }
        wrefresh(popup);

        // Handle input
        int input = wgetch(popup);
        switch (input) {
            case KEY_UP:
                highlight = (highlight == 0) ? categories.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == categories.size() - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
        }
    }

    delwin(popup);
    clear();
    refresh();

    if (choice != -1) {
        moveTask(taskId, categoriesType[choice]);
    }
}

int TaskManager::nextFree() {
    int maxId = 0;

    // Iterate through all task vectors
    for (const auto& taskList : tasks) {
        for (const auto& task : taskList) {
            maxId = std::max(maxId, task.getId());
        }
    }

    return maxId + 1;
}

void TaskManager::swapIn(){
    currentSelected = 0;
    currentType = 0;
}

void TaskManager::swapOut(){
    currentSelected = -1;
    currentType = -1;
}