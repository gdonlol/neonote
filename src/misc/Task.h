#ifndef TASK_H
#define TASK_H

#include <string>

// The Task class represents an individual task in a Kanban board.
class Task {
public:
    // Constructor: Initializes a task with an ID, title, description, and due date.
    Task(int id, const std::string& title, const std::string& description, const std::string& dueDate);
    
    // Returns the unique ID of the task
    int getId() const;

    // Returns the task's title
    std::string getTitle() const;

    // Returns the task's description
    std::string getDescription() const;

    // Returns the current status of the task (e.g., "To Do", "In Progress", "Done")
    std::string getStatus() const;

    // Returns the due date of the task
    std::string getDueDate() const;

    // Setter methods:
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setStatus(const std::string& newStatus);
    void setDueDate(const std::string& newDueDate);
    
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string dueDate;
};

#endif // TASK_H