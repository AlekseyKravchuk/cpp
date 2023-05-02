#include "database.h"

void Database::Add(const Date& date, const std::string& event) {
    _date2events[date].push_back(event);
}

void Database::Print(std::ostream& os) const {
    for (const auto& [date, events] : _date2events) {
        for (const auto& event : events) {
            os << date << ' ' << event << std::endl;
        }
    }
}

std::string Database::Last(const Date& date) {
    // TODO: now it's just stub
    return ""s;
}