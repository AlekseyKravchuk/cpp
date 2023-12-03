#include <iostream>
#include <set>
#include <vector>

#include "iterator_range.h"

int main() {
    const std::set<int> numbers = {6, 4, 7, 1, 2, 5};

    // Этот цикл выводит "1 2 4"
    for (int x : Head(numbers, 3)) {
        std::cout << x << ' ';
    }

    std::cout << '\n';

    const std::vector<int> numbers_v = {6, 4, 7, 1, 2, 5};

    // Этот цикл выводит "6 4 7"
    for (int x : Head(numbers_v, 3)) {
        std::cout << x << ' ';
    }
}