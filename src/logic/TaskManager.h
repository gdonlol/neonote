#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>

class TaskManager {
public:
    void addTask(const Task& task);
    void moveTask(int taskId, const std::string& newStatus);
    void printTasks() const;
    
private:
    std::vector<Task> tasks;
};

#endif // TASKMANAGER_H
