#include <ctime>
#include <iostream>

using namespace std::literals;

// Определяет количество високосных годов в диапазоне [_year_start, year]
int LeapYearsCount(int year) {
    // год является високосным, если он:
    // 1) делится на 4, но не делится на 100;
    // 2) кратен 400.
    return (year / 4) - (year / 100) + (year / 400);
}

int main() {
    // std::tm tm{};
    // tm.tm_year = 2022 - 1900;
    // tm.tm_mday = 1;

    // std::mktime(&tm);
    // std::cout << std::asctime(&tm)
    //           << "sizeof(std::tm) = "
    //           << sizeof(std::tm) << '\n';
    auto n1 = LeapYearsCount(1700);
    auto n2 = LeapYearsCount(2100);
    std::cout << "Numer of leap years in range [1700, 2100] = "s  << n2 - n1 << std::endl;
}