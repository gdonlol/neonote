#include "Task.h"
#include <sstream>

Task::Task(int id, const std::string& title, const std::string& description, const std::string& dueDate)
    : id(id), title(title), description(description), status("To Do"), dueDate(dueDate) {}

int Task::getId() const {
    return id;
}

std::string Task::getTitle() const {
    return title;
}

std::string Task::getDescription() const {
    return description;
}

std::string Task::getStatus() const {
    return status;
}

std::string Task::getDueDate() const {
    return dueDate;
}

void Task::setStatus(const std::string& newStatus) {
    status = newStatus;
}

std::string Task::toString() const {
    std::ostringstream oss;
    oss << "Task ID: " << id << ", Title: " << title 
        << ", Status: " << status << ", Due: " << dueDate;
    return oss.str();
}
