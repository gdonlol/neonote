#ifndef TASK_H
#define TASK_H

#include <string>

// The Task class represents an individual task in a Kanban board.
class Task {
public:
    // Constructor: Initializes a task with an ID, title, description, and due date.
    // The task's default status is set to "To Do".
    Task(int id, const std::string& title, const std::string& description, const std::string& dueDate);
    
    // Returns the unique ID of the task
    int getId() const;

    // Getter function for Task title
    std::string getTitle() const;

    // Returns the task's description
    std::string getDescription() const;

    // Returns the current status of the task (e.g., "To Do", "In Progress", "Done")
    std::string getStatus() const;

    // Returns the due date of the task
    std::string getDueDate() const;

    // Setter method to update the status of the task.
    void setStatus(const std::string& newStatus);
    
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string dueDate;
};

#endif // TASK_H