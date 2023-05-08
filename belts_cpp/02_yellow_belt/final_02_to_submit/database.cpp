#include "database.h"

void Database::Add(const Date &date, const std::string &event) {
    if (!_checker[date].count(event)) {
        _checker[date].insert(event);
        _d2e.emplace(date, event);
    }
}

void Database::Print(std::ostream &os) const {
    for (const auto& [date, event]: _d2e) {
        os << date << ' ' << event << std::endl;
    }
}

std::string Database::Last(const Date &date) const {
    const auto it = _d2e.upper_bound(date);
    if (it != _d2e.begin()) {
        std::ostringstream oss;
        oss << std::prev(it)->first << ' ' << std::prev(it)->second;
        return oss.str();
    } else {
        return "No entries"s;
    }
}

std::ostream& operator<<(std::ostream& os, const std::pair<const Date, std::string>& p) {
    os << p.first << ' ' << p.second;
    return os;
}