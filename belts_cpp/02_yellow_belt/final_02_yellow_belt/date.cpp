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

Date ParseDate(std::istringstream& iss) {
    bool state = true;

    int year;
    state = state && (iss >> year);
    state = state && (iss.peek() == '-');
    iss.ignore(1);

    int month;
    state = state && (iss >> month);
    state = state && (iss.peek() == '-');
    iss.ignore(1);

    int day;
    state = state && (iss >> day);
    state = state && iss.eof();  // iss.eof() <=> входных данных больше нет, завершаем ввод данных

    if (state) {
        return Date{year, month, day};
    } else {
        throw std::logic_error("Wrong date format: "s + iss.str());
    }
}