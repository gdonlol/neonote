#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>
#include <ncurses.h>

class TaskManager {
private:
    std::vector<Task> tasks;
public:
    TaskManager();
    void addTask(const Task& task);
    void removeTask(int taskId);
    std::vector<Task> getTasks() const;
    void updateTask(int taskId, const Task& updatedTask);
    // Render the list of tasks to the provided ncurses window starting at 'line'
    void renderTasks(WINDOW* win, int &line) const;
    void moveTask(int taskId, const std::string& newStatus);
};

#endif // TASKMANAGER_H
