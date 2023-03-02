#include <fstream>  // std::ifstream <=> typedef basic_ifstream<char> ifstream;
#include <iostream>
#include <string>  // std::getline
#include <vector>
#include <iomanip>

using namespace std::literals;

struct Date {
    int day{};
    int month{};
    int year{};
};

std::istream& operator>>(std::istream& is, Date& date) {
    is >> date.year;
    is.ignore(1, '-');
    is >> date.month;
    is.ignore(1, '-');
    is >> date.day;

    return is;
}

std::ostream& operator<<(std::ostream& os, Date& date) {
    os << std::setw(2) << std::setfill('0') << date.day;
    os << "."s;
    os << std::setw(2) << std::setfill('0') << date.month;
    os << "."s;
    os << std::setw(2) << std::setfill('0') << date.year;

    return os;
}

int main() {
    std::string input_file_name = "50_date.txt"s;
    std::ifstream input(input_file_name);
    Date date;

    // проверяем, что со входным потоком можно работать
    if (input) {
        while (input) {
            input >> date;
        }
        std::cout << date << std::endl;
        std::cout << "Done." << std::endl;
    } else {
        std::cout << "Error while opening file: \"" << input_file_name << "\"." << std::endl;
    }

    return 0;
}
