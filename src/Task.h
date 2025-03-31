#ifndef TASK_H
#define TASK_H

#include <string>

// The Task class represents an individual task in a Kanban board.
class Task {
public:
    // Constructor: Initializes a task with an ID, title, description, and due date.
    Task(int id, const std::string& title, const int status,
        const std::string& description = "");
    
    // Getter methods:
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    int getStatus() const;

    // Setter methods:
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setStatus(const int newStatus);
    
private:
    int id;
    std::string title;
    std::string description;
    int status;
};

#endif // TASK_H