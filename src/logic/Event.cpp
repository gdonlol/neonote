#include "Event.h"
#include <sstream>

Event::Event(int id, const std::string& title, const std::string& description, const std::string& startTime, const std::string& endTime)
    : id(id), title(title), description(description), startTime(startTime), endTime(endTime) {}

int Event::getId() const {
    return id;
}

std::string Event::getTitle() const {
    return title;
}

std::string Event::getDescription() const {
    return description;
}

std::string Event::getStartTime() const {
    return startTime;
}

std::string Event::getEndTime() const {
    return endTime;
}

void Event::setTitle(const std::string& title) {
    this->title = title;
}

void Event::setDescription(const std::string& description) {
    this->description = description;
}

void Event::setStartTime(const std::string& startTime) {
    this->startTime = startTime;
}

void Event::setEndTime(const std::string& endTime) {
    this->endTime = endTime;
}

std::string Event::toString() const {
    std::ostringstream oss;
    oss << "Event ID: " << id << ", Title: " << title 
        << ", Start: " << startTime << ", End: " << endTime;
    return oss.str();
}
