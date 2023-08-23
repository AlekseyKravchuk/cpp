#include <iostream>
#include <utility>
#include <vector>

#include "../../print.h"

// checking how std::move() works for whole std::vector

int main() {
    std::vector<int> source = {4, 5, 6, 7, 8};
    std::vector<int> target;
    target = std::move(source);

    std::cout << "source.size() = " << source.size()
              << ", source.capacity = " << source.capacity() << '\n';
    std::cout << "source content: " << source << std::endl;

    std::cout << std::endl;

    std::cout << "target.size() = " << target.size()
              << ", target.capacity = " << target.capacity() << '\n';
    std::cout << "target content: " << target << std::endl;

    return 0;
}
