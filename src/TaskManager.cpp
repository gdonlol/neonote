#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h> 

/**
 * @brief Constructor for TaskManager.
 * @param content Pointer to the ncurses window where tasks will be displayed.
 */
TaskManager::TaskManager(WINDOW* content) : content(content) {
    this->content = content;
}

/**
 * @brief Adds a task to the list.
 * @param task The task to add.
 */
void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

/**
 * @brief Removes a task by ID.
 * @param taskId The ID of the task to remove.
 */
void TaskManager::removeTask(int taskId) {
    auto it = std::remove_if(tasks.begin(), tasks.end(), 
                             [taskId](const Task& task) { return task.getId() == taskId; });

    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
    }
}

/**
 * @brief Updates an existing task.
 * @param taskId The ID of the task to update.
 * @param updatedTask The new task data.
 */
void TaskManager::updateTask(int taskId, const Task& updatedTask) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            task = updatedTask;
            return;
        }
    }
}

/**
 * @brief Moves a task to a new status.
 * @param taskId The ID of the task to move.
 * @param newStatus The new status for the task.
 */
void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            task.setStatus(newStatus);
            return;
        }
    }
}

/**
 * @brief Gets all tasks.
 * @return A reference to the task list.
 */
const std::vector<Task>& TaskManager::getTasks() const {
    return tasks;
}

/**
 * @brief Renders the tasks in the ncurses window.
 */
void TaskManager::renderTasks() {
    wclear(content);
    box(content, 0, 0);

    int maxY, maxX;
    getmaxyx(content, maxY, maxX); // Get the height and width of the window

    int y = 2;
    int xToDo = 2;           // Starting X position for "To Do" column
    int xInProgress = 40;     // Starting X position for "In Progress" column
    int xDone = 80;           // Starting X position for "Done" column

    // Column headers
    mvwprintw(content, y, xToDo, "--- To Do ---");
    mvwprintw(content, y, xInProgress, "--- In Progress ---");
    mvwprintw(content, y, xDone, "--- Done ---");

    // Initialize the Y position for tasks in each column
    int todoY = y + 2;
    int inProgressY = y + 2;
    int doneY = y + 2;

    // Loop through tasks and display them in the appropriate column based on status
    for (const auto& task : tasks) {
        if (task.getStatus() == "To Do") {
            // Check if we have enough space for the task
            if (todoY + 5 > maxY) {
                // If not enough space, move the content up (scroll)
                wrefresh(content);
                getch();  // Wait for user input to scroll
                wclear(content);
                box(content, 0, 0);
                todoY = y + 2; // Reset Y position for To Do column
                mvwprintw(content, y, xToDo, "--- To Do ---");
                mvwprintw(content, y, xInProgress, "--- In Progress ---");
                mvwprintw(content, y, xDone, "--- Done ---");
            }
            mvwprintw(content, todoY, xToDo, "ID: %d", task.getId());
            mvwprintw(content, todoY + 1, xToDo, "Title: %s", task.getTitle().c_str());
            mvwprintw(content, todoY + 2, xToDo, "Status: %s", task.getStatus().c_str());
            mvwprintw(content, todoY + 3, xToDo, "---------------------");
            todoY += 5;
        } else if (task.getStatus() == "In Progress") {
            // Check if we have enough space for the task
            if (inProgressY + 5 > maxY) {
                // If not enough space, move the content up (scroll)
                wrefresh(content);
                getch();  // Wait for user input to scroll
                wclear(content);
                box(content, 0, 0);
                inProgressY = y + 2; // Reset Y position for In Progress column
                mvwprintw(content, y, xToDo, "--- To Do ---");
                mvwprintw(content, y, xInProgress, "--- In Progress ---");
                mvwprintw(content, y, xDone, "--- Done ---");
            }
            mvwprintw(content, inProgressY, xInProgress, "ID: %d", task.getId());
            mvwprintw(content, inProgressY + 1, xInProgress, "Title: %s", task.getTitle().c_str());
            mvwprintw(content, inProgressY + 2, xInProgress, "Status: %s", task.getStatus().c_str());
            mvwprintw(content, inProgressY + 3, xInProgress, "---------------------");
            inProgressY += 5;
        } else if (task.getStatus() == "Done") {
            // Check if we have enough space for the task
            if (doneY + 5 > maxY) {
                // If not enough space, move the content up (scroll)
                wrefresh(content);
                getch();  // Wait for user input to scroll
                wclear(content);
                box(content, 0, 0);
                doneY = y + 2; // Reset Y position for Done column
                mvwprintw(content, y, xToDo, "--- To Do ---");
                mvwprintw(content, y, xInProgress, "--- In Progress ---");
                mvwprintw(content, y, xDone, "--- Done ---");
            }
            mvwprintw(content, doneY, xDone, "ID: %d", task.getId());
            mvwprintw(content, doneY + 1, xDone, "Title: %s", task.getTitle().c_str());
            mvwprintw(content, doneY + 2, xDone, "Status: %s", task.getStatus().c_str());
            mvwprintw(content, doneY + 3, xDone, "---------------------");
            doneY += 5;
        }
    }

    wrefresh(content);
}

// Function to prompt the user for task details and add it
void TaskManager::promptForTask() {
    char title[100];
    char status[20];

    // Ask for the task title
    mvwprintw(content, 1, 2, "Enter task title: ");
    wrefresh(content);
    echo();  // Allow input from the user
    wgetstr(content, title);
    noecho();

    // Ask for the task status
    mvwprintw(content, 3, 2, "Enter task status (To Do, In Progress, Done): ");
    wrefresh(content);
    echo();
    wgetstr(content, status);
    noecho();

    // Create a new task with the entered data
    int taskId = tasks.size() + 1; // Simple ID generation
    Task newTask(taskId, std::string(title), std::string(status));

    // Add the new task to the list
    addTask(newTask);

    // Redraw the task list
    renderTasks();
}