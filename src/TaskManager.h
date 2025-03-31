#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>
#include <ncurses.h> 

// TaskManager class manages a collection of Task objects
class TaskManager {
public:
    // Constructor that takes an ncurses window for rendering
    TaskManager(WINDOW* content);

    // Adds a new task to the task list
    void addTask(const Task& task);

    // Removes a task from the list based on its ID
    void removeTask(int taskId);

    // Updates a task by replacing it with an updated version
    void updateTask(int taskId, const Task& updatedTask);

    // Moves a task to a new status category
    void moveTask(int taskId, const std::string& newStatus);

    // Returns all tasks
    const std::vector<Task>& getTasks() const;

    // Renders all tasks
    void renderTasks();

    // Prompt user for task
    void promptForTask();

    // Listens for key press events for task input.
    void listenForInput();

    int nextFree();

private:
    std::vector<Task> tasks;
    WINDOW* content;
};

#endif // TASKMANAGER_H