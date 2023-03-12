#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>  // std::gcd
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <tuple>
#include <vector>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

class Date {
   public:
    Date() = default;
    Date(int year, int month, int day) {
        _year = year;

        if (month >= 1 && month <= 12) {
            _month = month;
        } else {
            std::ostringstream oss;
            oss << "Month value is invalid: "s << month;
            throw std::runtime_error(oss.str());
        }

        if (day >= 1 && day <= 31) {
            _day = day;
        } else {
            std::ostringstream oss;
            oss << "Day value is invalid: "s << day;
            throw std::runtime_error(oss.str());
        }
    }

    Date(std::tuple<int, int, int> tpl) : Date(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl)) {}

    int GetYear() const { return _year; }
    int GetMonth() const { return _month; }
    int GetDay() const { return _day; }

   private:
    int _year{};
    int _month{};
    int _day{};
};

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    stream << std::setw(4) << std::setfill('0') << date.GetYear() << "-"
           << std::setw(2) << std::setfill('0') << date.GetMonth() << "-"
           << std::setw(2) << std::setfill('0') << date.GetDay();
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()) {
        return lhs.GetYear() < rhs.GetYear();
    }

    if (lhs.GetMonth() != rhs.GetMonth()) {
        return lhs.GetMonth() < rhs.GetMonth();
    }

    return lhs.GetDay() < lhs.GetDay();
}

class Database {
   public:
    void AddEvent(const Date& date, const std::string& event) {
        // если данной даты ещё нет в БД =>  смело добавляем дату событие на эту дату
        if (_date2events.count(date) == 0) {
            _date2events[date].insert(event);
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
        int N{};
        if (_date2events.count(date)) {
            N = static_cast<int>(_date2events[date].size());
            _date2events.erase(date);
        }
        std::cout << "Deleted "s << N << " events"s << std::endl;
        return 0;
    }

    std::optional<const std::set<std::string>* const> Find(const Date& date) const {
        if (_date2events.count(date)) {
            return &_date2events.at(date);
        } else {
            return std::nullopt;
        }
    }

    void Print() const {
        for (const auto& [date, events] : _date2events) {
            for (const auto& event : events) {
                std::cout << date << ' ' << event << std::endl;
            }
        }
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

std::tuple<int, int, int> ParseDate(const std::string& date_as_str) {
    int day{}, month{}, year{};
    std::istringstream iss(date_as_str);

    std::ostringstream oss;
    oss << "Wrong date format: "s << date_as_str;
    std::string wrong_date_format = oss.str();

    if (iss >> year) {
        if (iss.peek() == '-') {
            iss.ignore(1);
            if (iss >> month) {
                if (iss.peek() == '-') {
                    iss.ignore(1);
                    if ((iss >> day >> std::ws).fail()) {
                        throw std::runtime_error(wrong_date_format);
                    }
                    std::string trash{};
                    if (iss >> trash) {
                        throw std::runtime_error(wrong_date_format);
                    }
                } else {
                    throw std::runtime_error(wrong_date_format);
                }
            } else {
                throw std::runtime_error(wrong_date_format);
            }
        } else {  // символ, следующий за годом не является '-'
            throw std::runtime_error(wrong_date_format);
        }
    } else {  // не удалось считать год
        throw std::runtime_error(wrong_date_format);
    }

    return {year, month, day};
}

void ProcessCommands(std::istream& is, Database& db) {
    std::string line;
    while (std::getline(is, line)) {
        std::istringstream iss(line);
        std::string command{}, date_as_str{}, event{};

        if (iss >> command) {
            if (str2command.count(command)) {
                switch (str2command[command]) {
                    case COMMAND::ADD: {
                        if (iss >> date_as_str >> event) {
                            // Date date(ParseDate(date_as_str));
                            db.AddEvent(Date{ParseDate(date_as_str)}, event);
                        }
                        break;
                    }
                    case COMMAND::DEL: {
                        if (iss >> date_as_str) {
                            Date date{ParseDate(date_as_str)};
                            if (iss >> event) {
                                db.DeleteEvent(date, event);
                            } else {
                                db.DeleteDate(date);
                            }
                        }
                        break;
                    }
                    case COMMAND::FIND: {
                        if (iss >> date_as_str) {
                            if (auto result = db.Find(Date{ParseDate(date_as_str)}); result.has_value()) {
                                if (const auto& events = *(result.value()); !events.empty()) {
                                    for (const auto& event : events) {
                                        std::cout << event << std::endl;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case COMMAND::PRINT: {
                        db.Print();
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
#ifdef _GLIBCXX_DEBUG
    std::string path = "input.txt";
    std::ifstream in(path);
    std::streambuf* cin_buf_bkp_ptr = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());
#endif  // _GLIBCXX_DEBUG

    Database db;
    try {
        ProcessCommands(std::cin, db);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

#ifdef _GLIBCXX_DEBUG
    std::cin.rdbuf(cin_buf_bkp_ptr);
#endif  //_GLIBCXX_DEBUG
    std::cin.rdbuf();

    return 0;
}