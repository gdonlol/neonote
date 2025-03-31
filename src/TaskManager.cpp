#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h> 
#include <cstring>

/**
 * @brief Constructor for TaskManager.
 * 
 * Initializes a TaskManager with the given ncurses window where tasks will be displayed.
 * 
 * @param content Pointer to the ncurses window where tasks will be displayed.
 */
TaskManager::TaskManager(WINDOW* content) : content(content), currentSelected(0), currentType(0){
    this->content = content;
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
}

/**
 * @brief Removes a task from the task list by its ID.
 * 
 * This function searches for a task by its ID and removes it from the task list.
 * 
 * @param taskId The ID of the task to be removed.
 */
void TaskManager::removeTask(int taskId) {
    for (auto& taskList : tasks) {
        for (auto it = taskList.begin(); it != taskList.end(); ++it) {
            if (it->getId() == taskId) {
                taskList.erase(it);
                return;
            }
        }
    }
}

/**
 * @brief Updates an existing task with new data.
 * 
 * This function searches for a task by its ID and replaces it with the provided
 * updated task data.
 * 
 * @param taskId The ID of the task to be updated.
 * @param updatedTask The new Task object containing updated task data.
 */
void TaskManager::updateTask(int taskId, const Task& updatedTask) {
    for (auto& taskList : tasks) {  
        for (auto& task : taskList) {  
            if (task.getId() == taskId) { 
                task = updatedTask;
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
        for (auto it = taskList.begin(); it != taskList.end(); ++it) {
            if (it->getId() == taskId) {
                tasks[type].push_back(*it);
                taskList.erase(it);
                return;
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
    mvwprintw(todoWin, 0, (colWidth - 10) / 2, "--- To Do ---");
    mvwprintw(inProgressWin, 0, (colWidth - 15) / 2, "--- In Progress ---");
    mvwprintw(doneWin, 0, (colWidth - 9) / 2, "--- Done ---");

    // Display tasks in each column
    for (int col = 0; col < 3; ++col) {
        WINDOW* drawWin = (col == 0) ? todoWin : (col == 1) ? inProgressWin : doneWin;

        int y = 2;
        for (int i = 0; i < tasks[col].size(); ++i) {
            if(currentType == col && currentSelected == i){ wattron(drawWin, COLOR_PAIR(3)); }
            mvwhline(drawWin, y++, 1, ACS_HLINE, colWidth - 2);
            if(currentType == col && currentSelected == i){ wattroff(drawWin, COLOR_PAIR(3)); }

            mvwprintw(drawWin, y++, 1, "%s", tasks[col][i].getTitle().c_str());
            mvwprintw(drawWin, y++, 1, "#%d", tasks[col][i].getId());
            ++y;
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
                currentSelected = (currentSelected - 1 + tasks[currentType].size()) % tasks[currentType].size();
            }
            break;

        case 1:  // Down
            if (!tasks[currentType].empty()) {
                currentSelected = (currentSelected + 1) % tasks[currentType].size();
            }
            break;

        case 2:  // Left
            currentType = (currentType - 1 + 3) % 3;  // Circular navigation for columns
            currentSelected = std::min(currentSelected, (int)tasks[currentType].size() - 1);
            break;

        case 3:  // Right
            currentType = (currentType + 1) % 3;  // Circular navigation for columns
            currentSelected = std::min(currentSelected, (int)tasks[currentType].size() - 1);
            break;
    }
}

Task TaskManager::getSelectedTask(){
    return tasks[currentType][currentSelected];
}

/**
 * @brief Prompts the user to move a task to a different status via a popup.
 * @param taskId The ID of the task to move.
 */
void TaskManager::moveTaskPopup(int taskId) {
    const std::vector<std::string> categories = {"To Do", "In Progress", "Completed"};
    const std::vector<int> categoriesType = {0, 1, 2};

    int highlight = 0;
    int choice = -1;

    // Get screen dimensions
    int maxY, maxX;
    getmaxyx(content, maxY, maxX);

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
    wrefresh(popup);  // Ensure the window is drawn

    while (choice == -1) {
        // Display options
        for (size_t i = 0; i < categories.size(); i++) {
            if (i == highlight) {
                wattron(popup, A_REVERSE); // Highlight current selection
            }
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
