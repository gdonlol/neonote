#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Adds a new task to the task list
void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

// Removes a task from the list based on its ID
void TaskManager::removeTask(int taskId) {
    std::vector<Task>::iterator it = std::remove_if(tasks.begin(), tasks.end(), [taskId](const Task& task) {
        return task.getId() == taskId;
    });

    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
    }
}

// Updates a task by replacing it with an updated version
void TaskManager::updateTask(int taskId, Task updatedTask) {
    for (std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {
            *it = updatedTask;
            return;
        }
    }
}

// Moves a task to a new status category
void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    for (std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {
            it->setStatus(newStatus);
            return;
        }
    }
}

// Returns a formatted string representation of all tasks
std::string TaskManager::printTasks() const {
    std::ostringstream oss;
    oss << "--- Kanban Board ---\n";

    for (std::vector<Task>::const_iterator it = tasks.begin(); it != tasks.end(); ++it) {
        oss << "Task ID: " << it->getId() 
            << "\nTitle: " << it->getTitle() 
            << "\nDescription: " << it->getDescription() 
            << "\nStatus: " << it->getStatus() 
            << "\nDue Date: " << it->getDueDate() 
            << "\n--------------------\n";
    }
    return oss.str();
}