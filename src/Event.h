#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    Event(int id, const std::string& title, const std::string& description, const std::string& date);
    std::string toString() const;
    
private:
    int id;
    std::string title;
    std::string description;
    std::string date; // Date is stored as a string for simplicity.
};

#endif // EVENT_H
