#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>  // std::numeric_limits<streamsize>::max()
#include <ostream>
#include <sstream>
#include <string>

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

void EnsureNextSymbolAndSkip(std::stringstream& stream) {
    if (stream.peek() != '/') {
        std::ostringstream oss;
        oss << "Expected '/', but has '"s << static_cast<char>(stream.peek()) <<"'; valid date format: \"YYYY/MM/DD\""s;
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
        // // будем читать дату из строки
        // std::string date_str = "2017/01/25   \tabc";  // ERROR: Reduntant characters are found after date
        std::string date_str = "2017a01/25";  // ERROR: Ivalid date 
        // std::string date_str = "2017/01/25"s;
        auto date = ParseDate(date_str);
        std::cout << date << std::endl;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
