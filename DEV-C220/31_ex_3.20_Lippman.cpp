#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    // std::istringstream iss("1 2 3 4 5 6 7 8"s);
    std::istringstream iss("1 2 3 4 5 6 7 8 9"s);
    std::vector<int> v;
    int val{};

    while (iss >> val) {
        v.push_back(val);
    }

    PrintCollection(v, "Initial state of vector: ");

    // // Отобразить сумму каждой пары соседних элементов
    // for (auto it = v.begin(); it != v.end(); ++it) {
    //     if (std::next(it) != v.end()) {
    //         std::cout << *it + *std::next(it) << " ";
    //         it = std::next(it);
    //     } else {
    //         std::cout << *it << " ";
    //     }
    // }
    // std::cout << std::endl;

    // Отобразить сумму первого и последнего, затем сумму второго и предпоследнего и т.д.
    // TODO
    for (auto it = v.begin(), itLast = std::prev(v.end()); it != v.end(); ++it) {
        if (std::next(it) != v.end()) {
            std::cout << *it + *std::next(it) << " ";
            it = std::next(it);
        } else {
            std::cout << *it << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
