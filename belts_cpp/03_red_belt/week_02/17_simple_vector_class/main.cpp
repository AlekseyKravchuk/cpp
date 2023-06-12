#include <algorithm>
#include <iostream>

#include "simple_vector.h"

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

    for (int elm : v) {
        std::cout << elm << ' ';
    }
    std::cout << std::endl;

    // благодаря тому, что мы дали нашему вектору итераторы (методы "begin" и "end"), мы можем использовать его в алгоритмах
    std::sort(v.begin(), v.end());

    std::cout << "sorted vector: ";
    for (int elm : v) {
        std::cout << elm << ' ';
    }
    std::cout << std::endl;

    return 0;
}
