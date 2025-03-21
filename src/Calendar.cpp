#include "Calendar.h"
#include <iostream>

void Calendar::addEvent(const Event& event) {
    events.push_back(event);
    std::cout << "Event added: " << event.toString() << std::endl;
}

void Calendar::printEvents() const {
    std::cout << "\n--- Calendar Events ---\n";
    for (const auto& event : events) {
        std::cout << event.toString() << std::endl;
    }
    std::cout << "-----------------------\n";
}
