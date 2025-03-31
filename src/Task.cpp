#include "Task.h"
#include <sstream>

/**
 * @brief Constructs a Task object.
 * 
 * This constructor initializes a task with an ID, title, status, description, and due date.
 * 
 * @param id The unique identifier for the task.
 * @param title The title of the task.
 * @param status The current status of the task (e.g., "in-progress", "completed").
 * @param description A brief description of the task.
 * @param dueDate The due date of the task.
 */
Task::Task(int id, const std::string& title, int status, 
    const std::string& description)
: id(id), title(title), status(status), description(description) {}

/**
 * @brief Gets the unique identifier for the task.
 * @return The ID of the task.
 */
int Task::getId() const { return id; }

/**
 * @brief Gets the title of the task.
 * @return The title of the task.
 */
std::string Task::getTitle() const { return title; }

/**
 * @brief Gets the description of the task.
 * @return The description of the task.
 */
std::string Task::getDescription() const { return description; }

/**
 * @brief Gets the current status of the task.
 * @return The status of the task.
 */
int Task::getStatus() const { return status; }

/**
 * @brief Updates the title of the task.
 * @param newTitle The new title for the task.
 */
void Task::setTitle(const std::string& newTitle) { title = newTitle; }

/**
 * @brief Updates the description of the task.
 * @param newDescription The new description for the task.
 */
void Task::setDescription(const std::string& newDescription) { description = newDescription; }

/**
 * @brief Updates the status of the task.
 * @param newStatus The new status for the task.
 */
void Task::setStatus(int newStatus) { status = newStatus; }