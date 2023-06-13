#include <algorithm>
#include <iostream>
#include <sstream>

#include "simple_vector.h"

using namespace std::literals;

template <typename Collection>
std::string Join(const Collection& collection, const std::string separator = " "s) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }
        isFirst = false;
        oss << elm;
    }

    return oss.str();
}

template <typename T>
void PrintVector(const SimpleVector<T>& v) {
    std::cout << Join(v, ", "s) << std::endl;
}

int main() {
    SimpleVector<int> v(5);

    // for (int i = 0; i < 5; ++i) {
    //     v[i] = 5 - i;
    // }

    v[0] = 55;
    v[1] = 20;
    v[2] = 33;
    v[3] = 24;
    v[4] = 66;

    // for (int i = 0; i < 5; ++i) {
    //     std::cout << v[i] << ' ';
    // }
    // std::cout << std::endl;

    // // благодаря тому, что в классе "SimpleVector" реализованы методы "begin" и "end", можем использовать "range-based" цикл "for"
    // for (int elm : v) {
    //     std::cout << elm << ' ';
    // }
    // std::cout << std::endl;
    PrintVector(v);

    // благодаря тому, что мы дали нашему вектору итераторы (методы "begin" и "end"), мы можем использовать его в алгоритмах
    std::sort(v.begin(), v.end());

    std::cout << "sorted vector: ";
    for (int elm : v) {
        std::cout << elm << ' ';
    }
    std::cout << std::endl;

    return 0;
}
