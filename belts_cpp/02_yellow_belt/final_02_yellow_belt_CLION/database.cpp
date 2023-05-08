#include "database.h"

void Database::Add(const Date &date, const std::string &event) {
    if (!_checker[date].count(event)) {
        _checker[date].insert(event);
        _d2e[date].push_back(event);
    }
}

void Database::Print(std::ostream &os) const {
    for (const auto& [date, events]: _d2e) {
        for (const auto& event: events) {
            os << date << ' ' << event << std::endl;
        }
    }
}

std::string Database::Last(const Date &date) const {
    const auto it_upper_bound = _d2e.upper_bound(date);
    if (it_upper_bound != _d2e.begin()) {
        std::ostringstream oss;
        auto it = std::prev(it_upper_bound);
        oss << it->first << ' ' << (it->second).back();
        return oss.str();
    } else {
        return "No entries"s;
    }
}

// для вывода результатов Database::FindIf
std::ostream& operator<<(std::ostream &os, const std::pair<Date, std::vector<std::string>>& p) {
    for (const auto& event : p.second) {
        os << p.first << ' ' << event;
    }

    return os;
}