#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h> 
#include <cstring>

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
    renderTasks();
}

/**
 * @brief Removes a task by ID.
 * @param taskId The ID of the task to remove.
 */
void TaskManager::removeTask(int taskId) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [taskId](const Task& t) {
        return t.getId() == taskId;
    });
    if (it != tasks.end()) {
        tasks.erase(it);
    }
    renderTasks();
}

/**
 * @brief Updates an existing task.
 * @param taskId The ID of the task to update.
 * @param updatedTask The new task data.
 */
void TaskManager::updateTask(int taskId, const Task& updatedTask) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [taskId](const Task& t) {
        return t.getId() == taskId;
    });
    if (it != tasks.end()) {
        *it = updatedTask;
    }
    renderTasks();
}

/**
 * @brief Moves a task to a new status.
 * @param taskId The ID of the task to move.
 * @param newStatus The new status for the task.
 */
void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [taskId](const Task& t) {
        return t.getId() == taskId;
    });
    if (it != tasks.end()) {
        it->setStatus(newStatus);
    }
    renderTasks();
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
            mvwprintw(content, todoY, xToDo, "ID: %d", task.getId());
            mvwprintw(content, todoY + 1, xToDo, "Title: %s", task.getTitle().c_str());
            mvwprintw(content, todoY + 2, xToDo, "Status: %s", task.getStatus().c_str());
            mvwprintw(content, todoY + 3, xToDo, "---------------------");
            todoY += 5;
        } else if (task.getStatus() == "In Progress") {
            mvwprintw(content, inProgressY, xInProgress, "ID: %d", task.getId());
            mvwprintw(content, inProgressY + 1, xInProgress, "Title: %s", task.getTitle().c_str());
            mvwprintw(content, inProgressY + 2, xInProgress, "Status: %s", task.getStatus().c_str());
            mvwprintw(content, inProgressY + 3, xInProgress, "---------------------");
            inProgressY += 5;
        } else if (task.getStatus() == "Done") {
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

    // Clear previous input display area
    wclear(content);  // Clear only the part of the screen where input is requested
    box(content, 0, 0);  // Reapply the border if needed
    mvwprintw(content, 1, 2, "Enter task title: ");
    mvwprintw(content, 3, 2, "Enter task status (To Do, In Progress, Done): ");
    wrefresh(content);

    // Ask for the task title
    mvwprintw(content, 2, 2, "Title: ");
    wrefresh(content);
    echo();  // Allow input from the user
    wgetstr(content, title);
    noecho();

    if (strlen(title) == 0) {
        mvwprintw(content, 5, 2, "Title cannot be empty! Press any key to retry.");
        wrefresh(content);
        getch();
        return;  // Exit the function if the title is empty
    }

    // Ask for the task status
    mvwprintw(content, 4, 2, "Status: ");
    wrefresh(content);
    echo();
    wgetstr(content, status);
    noecho();

    // Validate status input
    std::string validStatuses[] = {"To Do", "In Progress", "Done"};
    bool isValidStatus = std::find(std::begin(validStatuses), std::end(validStatuses), status) != std::end(validStatuses);

    if (!isValidStatus) {
        mvwprintw(content, 5, 2, "Invalid status! Press any key to retry.");
        wrefresh(content);
        getch();
        return;  // Exit if status is invalid
    }

    // Create a new task with the entered data
    int taskId = nextFree();  // Use nextFree to ensure a unique ID
    Task newTask(taskId, std::string(title), std::string(status));

    // Add the new task to the list
    addTask(newTask);

    // Clear input area and show task list again
    mvwprintw(content, 2, 2, "Task Added! Press any key to continue.");
    wrefresh(content);
    getch();

    // Redraw the task list
    renderTasks();
}

/**
 * @brief Listens for key press events, including Ctrl + 't' for task input.
 */
void TaskManager::listenForInput() {
    int ch;
    while ((ch = getch()) != KEY_EXIT) {  // Listen for a key press until KEY_EXIT
        // Debugging: Print the value of ch to see what key was pressed
        printw("Key pressed: %d\n", ch);
        refresh();
        
        if (ch == 20) {  // Check if Ctrl + T is pressed (ASCII value 20)
            promptForTask();  // Trigger the task input prompt
        }
    }
}

int TaskManager::nextFree() {
    int nextId = 1;
    for (const auto& task : tasks) {
        if (task.getId() == nextId) {
            nextId++;
        }
    }
    return nextId;
}