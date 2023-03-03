// TODO:
#include <fstream>
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

    int GetYear() const { return _year; }
    int GetMonth() const { return _month; }
    int GetDay() const { return _day; }

   private:
    int _year{};
    int _month{};
    int _day{};
};

bool operator<(const Date& lhs, const Date& rhs) {
}

class Database {
   public:
    void AddEvent(const Date& date, const std::string& event) {
        // если данной даты ещё нет в БД =>  смело добавляем дату событие на эту дату
        if (_date2events.count(date) == 0) {
            _date2events.at(date).insert(event);
        } else {  // если всё-таки дата уже использована в БД, добавляем событие, только если оно отсутствует
            if (_date2events.at(date).count(event) == 0) {
                _date2events.at(date).insert(event);
            }
        }
    }

    bool DeleteEvent(const Date& date, const std::string& event) {

    }

    int DeleteDate(const Date& date) {

    }

    auto Find(const Date& date) const {

    }

    void Print() const {

    }

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
}

std::istream& operator>>(std::istream& is, Date& date) {
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
                        /* code */
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
                    default:
                        break;
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