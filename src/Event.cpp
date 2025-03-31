#include "Event.h"
#include <sstream>

/**
 * @class Event
 * @brief Represents an event with an ID, title, description, and date.
 * 
 * This class provides methods to access and modify the event's details, such as its title, description,
 * and date. It also includes a method to convert the event's details into a string representation.
 */
Event::Event(int id, const std::string& title, const std::string& description, const std::string& date)
    : id(id), title(title), description(description), date(date) {}

/**
 * @brief Converts the event's details to a string representation.
 * 
 * @return A string representing the event, including its ID, title, date, and description.
 */
std::string Event::toString() const {
    std::ostringstream oss;
    oss << "Event ID: " << id << ", Title: " << title << ", Date: " << date << "]\n" << "   Description: " << description;
    return oss.str();
}

// GETTER METHODS
/**
 * @brief Returns the event's unique identifier.
 * 
 * @return The event's ID.
 */
int Event::getId() const { return id; }

/**
 * @brief Returns the title of the event.
 * 
 * @return The event's title.
 */
std::string Event::getTitle() const { return title; }

/**
 * @brief Returns the description of the event.
 * 
 * @return The event's description.
 */
std::string Event::getDescription() const { return description; }

/**
 * @brief Returns the date of the event.
 * 
 * @return The event's date.
 */
std::string Event::getDate() const { return date; }


// SETTER METHODS
/**
 * @brief Updates the event's title.
 * 
 * @param newTitle The new title to set for the event.
 */
void Event::setTitle(const std::string& newTitle) { title = newTitle; }

/**
 * @brief Updates the event's description.
 * 
 * @param newDescription The new description to set for the event.
 */
void Event::setDescription(const std::string& newDescription) { description = newDescription; }

/**
 * @brief Updates the event's date.
 * 
 * @param newDate The new date to set for the event.
 */
void Event::setDate(const std::string& newDate) { date = newDate; }