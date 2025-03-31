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
    void addTask(const std::string& title, int type);

    // Removes a task from the list based on its ID
    void removeTask(int taskId);

    // Updates a task by replacing it with an updated version
    void updateTask(int taskId, const Task& updatedTask);

    // Moves a task to a new status category
    void moveTask(int taskId, int type);

    // Returns all tasks
    const std::vector<std::vector<Task>>& getTasks() const;

    // Renders all tasks
    void renderTasks();


    int nextFree();

    void moveTaskPopup(int taskId);

    void moveSelection(int direction);
    
    int getSelectedTaskId();

private:
    std::vector<std::vector<Task>> tasks = {{}, {}, {}};
    WINDOW* content;

    int currentSelected;
    int currentType;
};

#endif // TASKMANAGER_H