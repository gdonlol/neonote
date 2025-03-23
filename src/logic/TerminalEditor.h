#include "TaskManager.h"
#include <ncurses.h>

TaskManager::TaskManager() {}

void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

void TaskManager::removeTask(int taskId) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {
            tasks.erase(it);
            break;
        }
    }
}

std::vector<Task> TaskManager::getTasks() const {
    return tasks;
}

void TaskManager::updateTask(int taskId, const Task& updatedTask) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            task = updatedTask;
            break;
        }
    }
}

void TaskManager::renderTasks(WINDOW* win, int &line) const {
    // Print a header for clarity
    mvwprintw(win, line++, 1, "---- To Do List ----");
    for (const auto& task : tasks) {
        mvwprintw(win, line++, 1, "%s", task.toString().c_str());
    }
    mvwprintw(win, line++, 1, "--------------------");
}

void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    for (auto & task : tasks) {
        if (task.getId() == taskId) {
            task.setStatus(newStatus);
            return;
        }
    }
}
