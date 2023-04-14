#include <cstdint>  // uint64_t
#include <iostream>
#include <limits>

using namespace std::literals;

class Date {
   public:
    Date() = default;
    void SetYear(int year) { _year = year; }
    void SetMonth(int month) { _month = month; }
    void SetDay(int day) { _day = day; }

   private:
    int _year = 2000;
    int _month = 1;
    int _day = 1;
    uint64_t _total_days = 0;  // количество дней начиная с 01.01.2000
};

// TODO:
// https://www.geeksforgeeks.org/count-of-leap-years-in-a-given-year-range/ - Count of Leap Years in a given year range
// Find number of days between two given dates - Find number of days between two given dates
std::istream& operator>>(std::istream& is, Date& date) {
    int day{0}, month{0}, year{0};

    is >> year;
    if (is.peek() != '-') {
        throw std::runtime_error("Wrong date format. Correct format is: YYYY-MM-DD"s);
    }
    is.ignore('-');

    is >> month;
    if (is.peek() != '-') {
        throw std::runtime_error("Wrong date format. Correct format is: YYYY-MM-DD"s);
    }

    is >> day;

    date.SetYear(year);
    date.SetMonth(month);
    date.SetDay(day);

    // + 1 дополнительный элемент для удобства счета
    const unsigned int daysUpToCurrentMonth[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    return is;
}

int GetDifferenceInDays(const Date& date_from, const Date& date_to) {
}

// Returns: "true" если "year" является високосным годом, в противном случае - "false"
// Определение високосного года:
//     - год, номер которого кратен 400, — високосный;
//     - остальные годы, номер которых кратен 100, — НЕвисокосные (например, годы 1700, 1800, 1900, 2100, 2200, 2300);
//     - остальные годы, номер которых кратен 4, — високосные[5];
//     - все остальные годы — невисокосные.
constexpr bool is_leap(int year) noexcept {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int LeapYearsBefore(int year) {
    year--;
    return (year / 4) - (year / 100) + (year / 400);
}

int main() {
    for (int year = 2000; year < 2100; ++year) {
        if (is_leap(year)) {
            std::cout << year << " "s;
        }
    }
    return 0;
}
