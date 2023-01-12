#include <algorithm>  // std::sort
#include <iomanip>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>
#include <vector>

#include "../my_print.h"

using namespace std::literals;

struct Duration {
    int total{};
    int hour{};
    int min{};

    Duration() = default;
    Duration(int h, int m) {
        total = h * 60 + m;
        hour = (total / 60) % 24;
        min = total % 60;
    }

    bool operator<(const Duration& other) {
        if (hour == other.hour) {
            return min < other.min;
        }
        return hour < other.hour;
    }
};

Duration operator+(const Duration& lhs, const Duration& rhs) {
    return {lhs.hour + rhs.hour, lhs.min + rhs.min};
}

std::istream& operator>>(std::istream& input_stream, Duration& duration) {
    input_stream >> duration.hour;
    input_stream.ignore();
    input_stream >> duration.min;

    return input_stream;
}

std::ostream& operator<<(std::ostream& output_stream, const Duration& duration) {
    output_stream << std::setfill('0');
    output_stream << std::setw(2) << duration.hour << ":" << std::setw(2) << duration.min;

    return output_stream;
}

int main() {
    // создаем строковый поток для удобства (чтобы не вводить с консоли)
    std::istringstream dur_ss1{"16 45"s};
    std::istringstream dur_ss2{"23 50"s};
    std::istringstream dur_ss3{"1 12"s};

    // создаем 2 интервала
    Duration dur_1;
    Duration dur_2;
    Duration dur_3;

    dur_ss1 >> dur_1;
    dur_ss2 >> dur_2;
    dur_ss3 >> dur_3;

    Duration dur_4 = dur_1 + dur_2 + dur_3;

    // хотим отсортировать временные интервалы
    std::vector<Duration> v{dur_1, dur_2, dur_3, dur_4};
    PrintCollection(v, "Исходное состояние вектора интервалов: "s);

    std::sort(v.begin(), v.end());

    // 
    // std::sort(v.begin(), v.end(),
    //           [](const Duration& lhs, const Duration& rhs) {
    //               if (lhs.hour == rhs.hour) {
    //                   return lhs.min < rhs.min;
    //               }
    //               return lhs.min < rhs.min;
    //           });

    // распечатываем интервалы после сортировки
    PrintCollection(v, "Состояние вектора интервалов после сортировки: "s);

    return 0;
}
