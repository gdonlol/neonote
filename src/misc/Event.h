#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    Event(int id, const std::string& title, const std::string& description, const std::string& date);
    std::string toString() const;

    // getters
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    std::string getDate() const { return date; }
    int getId() const { return id; }
    
private:
    int id;
    std::string title;
    std::string description;
    std::string date; // Date is stored as a string for simplicity.
};

#endif // EVENT_H
