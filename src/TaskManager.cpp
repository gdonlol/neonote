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

    int y = 2;
    mvwprintw(content, 1, 2, "--- Task List ---");

    for (const auto& task : tasks) {
        mvwprintw(content, y, 2, "ID: %d", task.getId());
        mvwprintw(content, y + 1, 2, "Title: %s", task.getTitle().c_str());
        mvwprintw(content, y + 2, 2, "Status: %s", task.getStatus().c_str());
        mvwprintw(content, y + 3, 2, "---------------------");

        y += 5;
    }

    wrefresh(content);
}