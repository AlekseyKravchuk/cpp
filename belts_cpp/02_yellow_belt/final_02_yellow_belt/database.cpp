#include "database.h"

void Database::Add(const Date& date, const std::string& event) {
    if (!_date_to_events[date].count(event)) {
        _date_to_events[date][event] = ++_counter;
        _date_to_ins_order[date][_counter] = event;
    }
}

void Database::Print(std::ostream& os) const {
    for (const auto& [date, insert_id_to_event] : _date_to_ins_order) {
        for (const auto& [_, event] : insert_id_to_event) {
            os << date << ' ' << event << std::endl;
        }
    }
}

std::string Database::Last(const Date& date) {
    const auto it = _date_to_ins_order.upper_bound(date);
    if (it != _date_to_ins_order.begin()) {
        std::ostringstream oss;
        Date d = std::prev(it)->first;
        std::string last_event = std::prev(_date_to_ins_order[d].end())->second;
        oss << d << ' ' << last_event;
        return oss.str();
    } else {
        return "No entries"s;
    };
}