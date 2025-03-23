#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>

// TaskManager class manages a collection of Task objects
class TaskManager {
public:
    // Adds a new task to the task list
    void addTask(const Task& task);

    // Removes a task from the list based on its ID
    void removeTask(int taskId);

    // Updates a task by replacing it with an updated version
    void updateTask(int taskId, Task updatedTask);

    // Moves a task to a new status category
    void moveTask(int taskId, const std::string& newStatus);

    // Returns a formatted string representation of all tasks
    std::string printTasks() const;
    
private:
    std::vector<Task> tasks;
};

#endif // TASKMANAGER_H