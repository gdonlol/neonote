/**
 * @file Event.h
 * @brief Defines the Event class representing a calendar event with details such as ID, title, description, date, start time, and end time.
 * 
 * @author Emily Atyeo
 * @author Gordon Xu
 */

#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    Event(int id, const std::string& title, const std::string& description, const std::string& date);
    
    std::string toString() const;

    // Getter Methods:
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDate() const;
    int getId() const;
    std::string getStartTime();
    std::string getEndTime();

    // Setter Methods:
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setDate(const std::string& newDate);
    void setStartTime(const std::string& newStartTime);
    void setEndTime(const std::string& newEndTime);
    
private:
    int id;
    std::string title;
    std::string description;
    std::string date; // Date stored as a string for simplicity.
    std::string startTime;
    std::string endTime;
};

#endif // EVENT_H