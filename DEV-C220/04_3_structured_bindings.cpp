#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

std::ostream& operator<<(std::ostream& os, std::pair<std::string, int> p) {
    /* const */ auto& [key, value] = p;
    os << "{"s << key << ", "s << value << "}";

    return os;
}

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    using namespace std::literals;

    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    std::map<std::string, int> coursesDurations = {
        {"OOP", 5400},
        {"STL", 4800},
        {"Iterators", 5000}};

    PrintRange(coursesDurations.begin(), coursesDurations.end());
    // метод std::map::insert принимает ПАРУ {ключ, значение}
    // и возвращает тоже ПАРУ {[итератор на вставленную пару, или на существующую пару], признак была ли вставка успешной - true или false}
    // std::pair<std::map<std::string, int>::iterator, bool> res = coursesDurations.insert(std::pair<std::string, int>("AAA", 4200));
    // начиная с С++17 можно так
    auto [it, flag] = coursesDurations.insert({"AAA", 4200});

    PrintRange(coursesDurations.begin(), coursesDurations.end());
    // std::cout << res.second << std::endl;

    std::cout << std::boolalpha << flag << std::endl;

    auto [it_2, flag_2] = coursesDurations.insert({"AAA", 4200});
    PrintRange(coursesDurations.begin(), coursesDurations.end());
    std::cout << std::boolalpha << flag_2 << std::endl;

    // выводим map'у с помощью range-based for
    std::cout << "выводим map с помощью range-based for"s << std::endl;
    bool isFirst = true;
    for (const auto& [courseName, duration] : coursesDurations) {
        if (isFirst) {
            std::cout << "{"s << courseName << ", "s << duration << "}"s << std::flush;
            isFirst = false;
        } else {
            std::cout << ", {"s << courseName << ", "s << duration << "}"s << std::flush;
        }
    }
    std::cout << std::endl;


    return 0;
}
