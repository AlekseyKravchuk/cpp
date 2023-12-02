#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>  // std::numeric_limits<streamsize>::max()
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>  // std::tie

using namespace std::literals;

struct Date {
    int year{};
    int month{};
    int day{};

    std::string ToStr() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << day << '.'
            << std::setw(2) << month << '.'
            << std::setw(4) << year;

        return oss.str();
    }
};

bool operator<(const Date& lhs, const Date& rhs) {
    // !!! "std::tie" возвращает КОРТЕЖ ИЗ ССЫЛОК на элементы, которые были переданы ей в качестве параметров
    return std::tie(lhs.year, lhs.month, lhs.day) < std::tie(rhs.year, rhs.month, rhs.day);
}

void EnsureNextSymbolAndSkip(std::stringstream& stream) {
    if (stream.peek() != '/') {
        std::ostringstream oss;
        oss << "Expected '/', but has '"s << static_cast<char>(stream.peek()) << "'; valid date format: \"YYYY/MM/DD\""s;
        throw std::runtime_error(oss.str());
    }
    stream.ignore(1);
}

// заведем функцию ParseDate, которая будет возвращать дату
Date ParseDate(const std::string& s) {
    std::stringstream stream(s);
    Date date;

    stream >> date.year;
    EnsureNextSymbolAndSkip(stream);
    stream >> date.month;
    EnsureNextSymbolAndSkip(stream);
    stream >> date.day >> std::ws;

    // stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // std::cout << "lstream.rdbuf()->in_avail(): "s << stream.rdbuf()->in_avail() << std::endl;

    if (stream.rdbuf()->in_avail() != 0) {
        const std::string error_msg = "Reduntant characters are found after date \""s + date.ToStr() + "\""s;
        throw std::runtime_error(error_msg);
    }

    return date;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.ToStr();
    return os;
}

int main() {
    try {
        std::string date_str1 = "2017/01/25";
        std::string date_str2 = "2017/06/8";

        auto date1 = ParseDate(date_str1);
        auto date2 = ParseDate(date_str2);
        std::cout << "date1 = "s << date1 << std::endl;
        std::cout << "date2 = "s << date2 << std::endl;

        std::cout << std::boolalpha << "date1 < date2: "s << (date1 < date2) << std::endl;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
