#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
public:
    Task(int id, const std::string& title, const std::string& description, const std::string& dueDate);
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getDueDate() const;
    void setStatus(const std::string& newStatus);
    std::string toString() const;
    
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string dueDate;
};

#endif // TASK_H
