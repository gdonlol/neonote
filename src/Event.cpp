#include "Event.h"
#include <sstream>

// Constructor definition
Event::Event(int id, const std::string& title, const std::string& description, const std::string& date)
    : id(id), title(title), description(description), date(date) {}

// toString method definition
std::string Event::toString() const {
    std::ostringstream oss;
    oss << "Event ID: " << id << ", Title: " << title << ", Date: " << date << "]\n" << "   Description: " << description;
    return oss.str();
}

// GETTER METHODS
// Returns the event's unique identifier
int Event::getId() const { return id; }

// Returns the title of the event
std::string Event::getTitle() const { return title; }

// Returns the description of the event
std::string Event::getDescription() const { return description; }

// Returns the date of the event
std::string Event::getDate() const { return date; }


// SETTER METHODS
// Updates the event's title
void Event::setTitle(const std::string& newTitle) { title = newTitle; }

// Updates the event's description
void Event::setDescription(const std::string& newDescription) { description = newDescription; }

// Updates the event's date
void Event::setDate(const std::string& newDate) { date = newDate; }