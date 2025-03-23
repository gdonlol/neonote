#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string dueDate; // For simplicity, using string instead of DateTime
public:
    Task(int id, const std::string& title, const std::string& description, const std::string& status, const std::string& dueDate);
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getDueDate() const;
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setStatus(const std::string& status);
    void setDueDate(const std::string& dueDate);
    std::string toString() const;
};

#endif // TASK_H
