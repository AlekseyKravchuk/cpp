#include <algorithm>  // std::sort
#include <exception>  // std::exception
#include <iomanip>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>
#include <vector>

#include "../my_print.h"

using std::string;

struct Date {
    int year{};
    int month{};
    int day{};
};

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(2) << std::setfill('0') << date.day << "."
       << std::setw(2) << date.month << "."
       << std::setw(2) << date.year;

    return os;
}

void EnsureNextSymbolAndSkip(std::istringstream& iss) {
    // у потока есть метод "peek", он позволяет посмотреть, какой следующий символ содержится в потоке
    if (iss.peek() != '/') {
        std::ostringstream oss;
        oss << "expected \'/\', but has: "
            << "\'"s << static_cast<char>(iss.peek()) << "\'"s;
        // throw std::invalid_argument("Wrong date format. Desired format is: \"YYYY/MM/DD\"."s);

        // метод "str" вернет строку из потока, которая в нем записана
        throw std::runtime_error(oss.str());
    }
    iss.ignore(1);
}

Date ParseDate(const string& s) {
    std::istringstream iss(s);
    Date date;

    iss >> date.year;
    EnsureNextSymbolAndSkip(iss);

    iss >> date.month;
    EnsureNextSymbolAndSkip(iss);

    iss >> date.day;

    return date;
}

int main() {
    // string date_str = "2017/01/25"s;
    // string date_str = "2017/1/5"s;
    string date_str = "2017:b2/5"s;
    Date date;
    try {
        date = ParseDate(date_str);
    } catch (const std::exception& e) {
        // все исключения в С++ наследуются от класса std::exception,
        // поэтому мы можем все исключения ловить по константной ссылке "const std::exception&"
        std::cerr << e.what() << '\n';
    }

    std::cout << date << std::endl;

    return 0;
}
