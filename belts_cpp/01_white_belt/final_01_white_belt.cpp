// TODO:
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <vector>

using namespace std::literals;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
   public:
    Date() = default;
    Date(int year, int month, int day) {
        _year = year;

        if (month >= 1 && month <= 12) {
            _month = month;
        } else {
            std::ostringstream oss;
            oss << "Month value is invalid: s" << month;
            throw std::runtime_error(oss.str());
        }

        if (day >= 1 || day <= 31) {
            _day = day;
        } else {
            std::ostringstream oss;
            oss << "Day value is invalid: : s" << day;
            throw std::runtime_error(oss.str());
        }
    }

    int GetYear() const { return _year; }
    int GetMonth() const { return _month; }
    int GetDay() const { return _day; }

   private:
    int _year{};
    int _month{};
    int _day{};
};

class Database {
   public:
    void AddEvent(const Date& date, const std::string& event) {
        // если данной даты ещё нет в БД =>  смело добавляем дату событие на эту дату
        if (_date2events.count(date) == 0) {
            _date2events.at(date).insert(event);
        } else {  // если всё-таки дата уже использована в БД, добавляем событие, только если такое отсутствует
            if (_date2events.at(date).count(event) == 0) {
                _date2events.at(date).insert(event);
            }
        }
    }

    bool DeleteEvent(const Date& date, const std::string& event) {
        if (_date2events.count(date)) {
            if (_date2events[date].count(event)) {
                _date2events[date].erase(event);
                std::cout << "Deleted successfully"s << std::endl;
                return true;
            } else {
                std::cout << "Event not found"s << std::endl;
                return false;
            }
        } else {
            std::cout << "Event not found"s << std::endl;
            return false;
        }
    }

    int DeleteDate(const Date& date) {
        if (_date2events.count(date)) {
            int N = _date2events[date].size();
            std::cout << "Deleted "s << N << " events"s << std::endl;
            _date2events.erase(date);
            return N;
        }
        return 0;
    }

    auto Find(const Date& date) const {
        if (_date2events.count(date)) {
            for (const auto& event : _date2events.at(date)) {
                std::cout << event << std::endl;
            }
        }
    }

    void Print() const {
        Date d{1998, 11, 12};
        std::cout << d << std::endl;
        // for (const auto& [date, events] : _date2events) {
        //     for (const auto& event : events) {
        //         std::cout << date << ' ' << event << std::endl;
        //     }
        // }
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& date);

   private:
    std::map<Date, std::set<std::string>> _date2events;
};

enum class COMMAND {
    ADD,
    DEL,
    FIND,
    PRINT
};

std::map<std::string, COMMAND> str2command{
    {"Add"s, COMMAND::ADD},
    {"Del"s, COMMAND::DEL},
    {"Find"s, COMMAND::FIND},
    {"Print"s, COMMAND::PRINT},
};

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(4) << std::setfill('0') << date.GetYear()
       << std::setw(2) << std::setfill('0') << date.GetMonth()
       << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

std::istream& operator>>(std::istream& is, Date& date) {
    int year{}, month{}, day{};
    if (is >> year) {
        if (year < 1 && year > 12) {
            std::ostringstream oss;
            oss << "Month value is invalid: s" << year;
            throw std::runtime_error(oss.str());
        }
    }

    return is;
}

bool operator<(const Date& lhs, const Date& rhs) {
    return true;
}

void ProcessCommands(Database& db) {
    std::string line;
    while (getline(std::cin, line)) {
        std::istringstream iss(line);
        Date date{};
        std::string command{}, event{};

        if (iss >> command >> date >> event) {
            if (str2command.count(command)) {
                switch (str2command[command]) {
                    case COMMAND::ADD: {
                        db.AddEvent(date, event);
                        break;
                    }
                    case COMMAND::DEL: {
                        if (event.empty()) {
                            db.DeleteDate(date);
                        } else {
                            db.DeleteEvent(date, event);
                        }
                        break;
                    }
                    case COMMAND::FIND: {
                        /* code */
                        break;
                    }
                    case COMMAND::PRINT: {
                        /* code */
                        break;
                    }
                    default: {
                        std::cout << "Unknown command: "s << command << std::endl;
                        break;
                    }
                }
            } else {
                std::ostringstream oss;
                oss << "Unknown command: " << command;
                throw std::runtime_error(oss.str());
            }
        }
    }
}

int main() {
    Database db;
    try {
        ProcessCommands(db);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}