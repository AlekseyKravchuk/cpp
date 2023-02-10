#include <iostream>
#include <string>
#include <sstream>    // std::stringstream

using namespace std::literals;

constexpr unsigned long long operator""_hours(unsigned long long n) {
    return n * 60 * 60;
}

// std::string operator""_km(unsigned long long n) {
//     return std::to_string(n) + " kilometers"s;
// }

std::string operator""_km(unsigned long long n) {
    std::stringstream ss;
    ss << n;

    std::string str;
    ss >> str;

    return str + " kilometers"s; 
}

int main() {
    // хочется сделать так, чтобы программист мог задавать интервалы времени в часах,
    // но чтобы программа оперировала этими интервалами в секундах
    int interval_sec = 24_hours;
    std::cout << interval_sec << " seconds"s << std::endl;

    // или хочется вывести на консоль 100км
    std::cout << 100_km << std::endl;

    return 0;
}