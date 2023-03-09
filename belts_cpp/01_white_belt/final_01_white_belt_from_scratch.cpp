#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

class Date {
   public:
    Date(int new_year, int new_month, int new_day) {
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

    int GetYear() const {
        return _year;
    }

    int GetMonth() const {
        return _month;
    }

    int GetDay() const {
        return _day;
    }

   private:
    int _year;
    int _month;
    int _day;
};

// определить оператор сравнения для "Date" необходимо для использования этого типа в качестве ключей словаря
bool operator<(const Date& lhs, const Date& rhs) {
    // воспользуемся тем фактом, что векторы уже можно сравнивать на "<":
    // создадим вектора из года, месяца и дня для каждой даты и сравним их
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
           std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

// вывод даты в требуемом формате (перегрузка оператора вывода)
std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(4) << std::setfill('0') << date.GetYear() << '-'
       << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
       << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

class Database {
   public:
    void AddEvent(const Date& date, const std::string& event) {
        _date2events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const std::string& event) {
        if (_date2events.count(date) && _date2events[date].count(event)) {
            _date2events[date].erase(event);
            return true;
        } else {
            return false;
        }
    }

    int DeleteDate(const Date& date) {
        if (_date2events.count(date)) {
            const int event_count = _date2events[date].size();
            _date2events.erase(date);
            return event_count;
        } else {
            return 0;
        }
    }

    void Print() const {
        for (const auto& [date, events] : _date2events) {
            for (const auto& event : events) {
                std::cout << date << ' ' << event << std::endl;
            }
        }
    }

    std::set<std::string> Find(const Date& date) const {
        return (_date2events.count(date)) ? _date2events.at(date) : std::set<std::string>{};
    }

   private:
    std::map<Date, std::set<std::string>> _date2events;
};

Date ParseDate(const std::string date_as_str) {
    std::istringstream iss(date_as_str);
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
        throw std::logic_error("Wrong date format: "s + date_as_str);
    }
}

void ProcessCommands(std::istream& is, Database& db) {
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

    std::string line;
    while (std::getline(is, line)) {
        std::istringstream iss(line);
        std::string command{};

        if (iss >> command) {
            if (str2command.count(command)) {
                switch (str2command[command]) {
                    case COMMAND::ADD: {
                        std::string date_as_str, event;
                        if (iss >> date_as_str >> event) {
                            db.AddEvent(Date{ParseDate(date_as_str)}, event);
                        }
                        break;
                    }
                    case COMMAND::DEL: {
                        std::string date_as_str, event;
                        if (iss >> date_as_str) {
                            Date date{ParseDate(date_as_str)};

                            if (iss >> event) {
                                if (db.DeleteEvent(date, event)) {
                                    std::cout << "Deleted successfully"s << std::endl;
                                } else {
                                    std::cout << "Event not found"s << std::endl;
                                }
                            } else {
                                int event_count = db.DeleteDate(date);
                                std::cout << "Deleted "s << event_count << " events"s << std::endl;
                            }
                            break;
                        }
                    }
                    case COMMAND::FIND: {
                        std::string date_as_str;
                        if (iss >> date_as_str) {
                            if (auto events = db.Find(Date{ParseDate(date_as_str)}); !events.empty()) {
                                for (const auto& event : events) {
                                    std::cout << event << std::endl;
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
                throw std::runtime_error("Unknown command: "s + command);
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