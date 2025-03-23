#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
private:
    int id;
    std::string title;
    std::string description;
    std::string startTime; // For simplicity, using string instead of a DateTime class
    std::string endTime;
public:
    Event(int id, const std::string& title, const std::string& description, const std::string& startTime, const std::string& endTime);
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setStartTime(const std::string& startTime);
    void setEndTime(const std::string& endTime);
    std::string toString() const;
};

#endif // EVENT_H
