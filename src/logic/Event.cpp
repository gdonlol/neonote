#include "Event.h"
#include <sstream>

Event::Event(int id, const std::string& title, const std::string& description, const std::string& date)
    : id(id), title(title), description(description), date(date) {}

std::string Event::toString() const {
    std::ostringstream oss;
    oss << "Event ID: " << id << ", Title: " << title << ", Date: " << date << "]\n" << "   Description: " << description;
    return oss.str();
}
