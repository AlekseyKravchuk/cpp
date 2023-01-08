#include <iostream>

struct Day {
    int value;
    // исключаем возможность неявного преобразования типа "int" к структуре "Day" посредством explicit-конструктора
    explicit Day(int new_value) : value(new_value) {}
};

struct Month {
    int value;

    // исключаем возможность неявного преобразования типа "int" к структуре "Month" посредством explicit-конструктора
    explicit Month(int new_value) : value(new_value) {}
};

struct Year {
    int value;

    // исключаем возможность неявного преобразования типа "int" к структуре "Year" посредством explicit-конструктора
    explicit Year(int new_value) : value(new_value) {}
};

struct Date {
    int day;
    int month;
    int year;

    Date(Day newDay, Month newMonth, Year newYear)
        : day(newDay.value), month(newMonth.value), year(newYear.value) {}
};

void PrintDate(const Date& date) {
    using namespace std::literals;

    std::cout << date.day << "."s << date.month << "."s << date.year << std::endl;
}

int main() {
    Date date = {Day{10}, Month{11}, Year{2023}};
    // Date date = {{10}, {11}, {2023}};
    // Date date = {10, 11, 2023};
    PrintDate(date);

    return 0;
}
