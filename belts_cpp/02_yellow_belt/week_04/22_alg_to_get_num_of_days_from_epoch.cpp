#include <cstdint>  // uint64_t
#include <iostream>
#include <limits>
#include <numeric>  // std::accumulate
#include <sstream>
#include <string>

using namespace std::literals;

class Date {
   public:
    Date() = default;
    void SetDay(int day) {
    }

    void SetMonth(int month) {
    }

    void SetYear(int year) {
    }

   private:
    int _day;
    int _month;
    int _year;

    uint64_t _days_from_epoch{1};  // количество дней, начиная с 01.01.1700
};

// Returns: "true" если "year" является високосным годом, в противном случае - "false"
// Определение високосного года:
//     - год, номер которого кратен 400, — високосный;
//     - остальные годы, номер которых кратен 100, — невисокосные (например, годы 1700, 1800, 1900, 2100, 2200, 2300);
//     - остальные годы, номер которых кратен 4, — високосные[5];
//     - все остальные годы — невисокосные.
constexpr bool is_leap(int year) noexcept {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

std::istream& operator>>(std::istream& is, Date& date) {
    int day{0};
    int month{0};
    int year{0};

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

    return is;
}

uint64_t GetDaysFromEpoch(const std::string& date) {
}

int main() {
}