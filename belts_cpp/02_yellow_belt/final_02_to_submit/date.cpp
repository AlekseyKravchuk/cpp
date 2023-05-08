#include "date.h"

Date::Date(int new_year, int new_month, int new_day) {
    _year = new_year;

    if (new_month > 12 || new_month < 1) {
        throw std::logic_error("Month value is invalid: "s + std::to_string(new_month));
    }
    _month = new_month;

    if (new_day > 31 || new_day < 1) {
        throw std::logic_error("Day value is invalid: "s + std::to_string(new_day));
    }
    _day = new_day;
}

int Date::GetYear() const {
    return _year;
}

int Date::GetMonth() const {
    return _month;
}

int Date::GetDay() const {
    return _day;
}

bool Date::operator<(const Date& rhs) const {
    return std::tuple(_year, _month, _day) < std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool Date::operator<=(const Date& rhs) const {
    return std::tuple(_year, _month, _day) <= std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool Date::operator>(const Date& rhs) const {
    return std::tuple(_year, _month, _day) > std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool Date::operator>=(const Date& rhs) const {
    return std::tuple(_year, _month, _day) >= std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool Date::operator==(const Date& rhs) const {
    return std::tuple(_year, _month, _day) == std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool Date::operator!=(const Date& rhs) const {
    return std::tuple(_year, _month, _day) != std::tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

Date ParseDate(std::istream& is) {
    bool state = true;

    int year;
    state = state && (is >> year);
    state = state && (is.peek() == '-');
    is.ignore(1);

    int month;
    state = state && (is >> month);
    state = state && (is.peek() == '-');
    is.ignore(1);

    int day;
    state = state && (is >> day);

    if (state) {
        return Date{year, month, day};
    } else {
        throw std::logic_error("Wrong date format: "s);
    }
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(4) << std::setfill('0') << date.GetYear() << '-'
       << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
       << std::setw(2) << std::setfill('0') << date.GetDay();
    
    return os;
}