#include "TaskManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ncurses.h> 
#include <cstring>

/**
 * @brief Constructor for TaskManager.
 * 
 * Initializes a TaskManager with the given ncurses window where tasks will be displayed.
 * 
 * @param content Pointer to the ncurses window where tasks will be displayed.
 */
TaskManager::TaskManager(WINDOW* content) : content(content) {
    this->content = content;
}

/**
 * @brief Adds a task to the task list with the given title.
 * 
 * This function creates a new task with a unique ID, default status ("To Do"), 
 * and the given title. The task is then added to the task list.
 * 
 * @param title The title of the task to be added.
 */
void TaskManager::addTask(const std::string& title) {
    int newTaskId = nextFree();
    std::string defaultStatus = "To Do";
    tasks.emplace_back(newTaskId, title, defaultStatus);
}

/**
 * @brief Adds an existing task to the task list.
 * 
 * This function adds an already created Task object to the list of tasks.
 * 
 * @param task The Task object to be added to the task list.
 */
void TaskManager::addTask(const Task& task) {
    tasks.push_back(task);
}

/**
 * @brief Removes a task from the task list by its ID.
 * 
 * This function searches for a task by its ID and removes it from the task list.
 * 
 * @param taskId The ID of the task to be removed.
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
 * @brief Updates an existing task with new data.
 * 
 * This function searches for a task by its ID and replaces it with the provided
 * updated task data.
 * 
 * @param taskId The ID of the task to be updated.
 * @param updatedTask The new Task object containing updated task data.
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
 * 
 * This function searches for a task by its ID and updates its status to the
 * provided new status.
 * 
 * @param taskId The ID of the task to be moved.
 * @param newStatus The new status to assign to the task.
 */
void TaskManager::moveTask(int taskId, const std::string& newStatus) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            task.setStatus(newStatus);  // Update task status
            renderTasks();              // Refresh UI
            return;
        }
    }
}

/**
 * @brief Gets a reference to the list of all tasks.
 * @return A reference to the vector containing all tasks.
 */
const std::vector<Task>& TaskManager::getTasks() const {
    return tasks;
}

/**
 * @brief Renders the tasks in the ncurses window.
 * 
 * This function clears the window and displays the list of tasks divided by their
 * status into three columns: "To Do", "In Progress", and "Done".
 */
void TaskManager::renderTasks() {
    wclear(content);
    box(content, 0, 0);

    int maxY, maxX;
    getmaxyx(content, maxY, maxX);  // Get the height and width of the window

    // Calculate dimensions for the columns
    int colWidth = (maxX - 2) / 3;        // Divide window into 3 equal parts
    int colHeight = maxY - 2;       // Leave space for borders

    // Create derwin windows for each column
    WINDOW* todoWin = derwin(content, colHeight, colWidth, 1, 1);
    WINDOW* inProgressWin = derwin(content, colHeight, colWidth, 1, colWidth + 1);
    WINDOW* doneWin = derwin(content, colHeight, colWidth, 1, 2 * colWidth + 1);

    // Render headers
    mvwprintw(todoWin, 0, (colWidth - 10) / 2, "--- To Do ---");
    mvwprintw(inProgressWin, 0, (colWidth - 15) / 2, "--- In Progress ---");
    mvwprintw(doneWin, 0, (colWidth - 9) / 2, "--- Done ---");

    // Y positions for tasks in each column
    int todoY = 2, inProgressY = 2, doneY = 2;

    // Loop through tasks and display them in the appropriate column
    for (const auto& task : tasks) {
        if (task.getStatus() == "To Do") {
            mvwprintw(todoWin, todoY, 1, "ID: %d", task.getId());
            mvwprintw(todoWin, todoY + 1, 1, "Title: %s", task.getTitle().c_str());
            mvwprintw(todoWin, todoY + 2, 1, "Status: %s", task.getStatus().c_str());
            mvwprintw(todoWin, todoY + 3, 1, "---------------------");
            todoY += 5;
        } else if (task.getStatus() == "In Progress") {
            mvwprintw(inProgressWin, inProgressY, 1, "ID: %d", task.getId());
            mvwprintw(inProgressWin, inProgressY + 1, 1, "Title: %s", task.getTitle().c_str());
            mvwprintw(inProgressWin, inProgressY + 2, 1, "Status: %s", task.getStatus().c_str());
            mvwprintw(inProgressWin, inProgressY + 3, 1, "---------------------");
            inProgressY += 5;
        } else if (task.getStatus() == "Done") {
            mvwprintw(doneWin, doneY, 1, "ID: %d", task.getId());
            mvwprintw(doneWin, doneY + 1, 1, "Title: %s", task.getTitle().c_str());
            mvwprintw(doneWin, doneY + 2, 1, "Status: %s", task.getStatus().c_str());
            mvwprintw(doneWin, doneY + 3, 1, "---------------------");
            doneY += 5;
        }
    }

    // Refresh all windows
    wrefresh(todoWin);
    wrefresh(inProgressWin);
    wrefresh(doneWin);
    wrefresh(content);

    // Clean up windows
    delwin(todoWin);
    delwin(inProgressWin);
    delwin(doneWin);
}

/**
 * @brief Prompts the user for task details and adds the task to the list.
 * 
 * This function prompts the user to enter a task's title and status, validates the
 * inputs, and adds the new task to the list.
 */
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

<<<<<<< HEAD
/**
 * @brief Listens for user input, including special key presses.
 * 
 * This function listens for key press events and triggers actions such as
 * prompting for a new task when the user presses Ctrl + T (ASCII value 20).
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

/**
 * @brief Prompts the user to move a task to a different status via a popup.
 * @param taskId The ID of the task to move.
 */
=======
>>>>>>> ecdd0c6 (asdasd)
void TaskManager::moveTaskPopup(int taskId) {
    const std::vector<std::string> categories = {"To Do", "In Progress", "Completed"};
    int highlight = 0;
    int choice = -1;

    // Get screen dimensions
    int maxY, maxX;
    getmaxyx(content, maxY, maxX);

    // Popup dimensions
    int popupHeight = categories.size() + 4;
    int popupWidth = 20;
    int startY = (maxY - popupHeight) / 2;
    int startX = (maxX - popupWidth) / 2;

    // Create popup window
    WINDOW* popup = newwin(popupHeight, popupWidth, startY, startX);
    box(popup, 0, 0);
    keypad(popup, TRUE);

    mvwprintw(popup, 1, 3, "Move Task To:");
    wrefresh(popup);  // Ensure the window is drawn

    while (choice == -1) {
        // Display options
        for (size_t i = 0; i < categories.size(); i++) {
            if (i == highlight) {
                wattron(popup, A_REVERSE); // Highlight current selection
            }
            mvwprintw(popup, i + 2, 3, "%s", categories[i].c_str());
            wattroff(popup, A_REVERSE);
        }

        wrefresh(popup);

        // Handle input
        int input = wgetch(popup);
        switch (input) {
            case KEY_UP:
                highlight = (highlight == 0) ? categories.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == categories.size() - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
        }
    }

    delwin(popup);
    clear();
    refresh();

    if (choice != -1) {
        moveTask(taskId, categories[choice]);
    }
}

/**
 * @brief Finds and returns the next available task ID.
 * @return The next available task ID.
 */
int TaskManager::nextFree() {
    int nextId = 1;
    for (const auto& task : tasks) {
        if (task.getId() == nextId) {
            nextId++;
        }
    }
    return nextId;
}