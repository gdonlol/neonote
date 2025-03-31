#ifndef TASK_H
#define TASK_H

#include <string>

// The Task class represents an individual task in a Kanban board.
class Task {
public:
    // Constructor: Initializes a task with an ID, title, description, and due date.
    Task(int id, const std::string& title, const std::string& status,
        const std::string& description = "", const std::string& dueDate = "");
    
    // Getter methods:
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getStatus() const;
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