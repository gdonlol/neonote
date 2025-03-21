#include "TaskManager.h"
#include <iostream>

void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
    std::cout << "Task added: " << task.toString() << std::endl;
}

void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            task.setStatus(newStatus);
            std::cout << "Task " << taskId << " moved to " << newStatus << std::endl;
            return;
        }
    }
    std::cout << "Task ID " << taskId << " not found." << std::endl;
}

void TaskManager::printTasks() const {
    std::cout << "\n--- Kanban Board ---\n";
    for (const auto& task : tasks) {
        std::cout << task.toString() << std::endl;
    }
    std::cout << "--------------------\n";
}
