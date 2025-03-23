#include "Task.h"
#include <sstream>

Task::Task(int id, const std::string& title, const std::string& description, const std::string& status, const std::string& dueDate)
    : id(id), title(title), description(description), status(status), dueDate(dueDate) {}

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

void Task::setTitle(const std::string& title) {
    this->title = title;
}

void Task::setDescription(const std::string& description) {
    this->description = description;
}

void Task::setStatus(const std::string& status) {
    this->status = status;
}

void Task::setDueDate(const std::string& dueDate) {
    this->dueDate = dueDate;
}

std::string Task::toString() const {
    std::ostringstream oss;
    oss << "Task ID: " << id << ", Title: " << title << ", Status: " << status << ", Due: " << dueDate;
    return oss.str();
}
