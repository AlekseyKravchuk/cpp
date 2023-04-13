#include <cstdint>  // uint64_t
#include <iostream>
#include <limits>

using namespace std::literals;

class Date {
   public:
    Date() = default;

   private:
    int _year = 1700;
    int _month = 1;
    int _day = 1;
    uint64_t _days_from_epoch;  // количество дней начиная с 01.01.1700
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

int main() {
    std::cout.imbue(std::locale(""));
    std::cout << "max value of type 'uint64_t': " << std::numeric_limits<uint64_t>::max() << std::endl;
    return 0;
}
