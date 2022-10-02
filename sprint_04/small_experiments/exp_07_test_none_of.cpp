// none_of example
#include <algorithm>  // std::none_of
#include <array>      // std::array
#include <iostream>   // std::cout
#include <vector>

int main() {
    // std::array<int, 8> foo = {1, 2, 4, 8, 16, 32, 64, 128};
    std::vector vec = {/* 1, 2, 4, 8, 16, 32,  */64, 128};

    // if (std::none_of(vec.begin(), vec.end(), [](int i) { return i < 0; }))
    //     std::cout << "There are no negative elements in the range.\n";

    if (std::none_of(vec.begin(), vec.end(), [](int i) { return i < 32; })) {
        std::cout << "There are no elements less than 32 in the range.\n";
    } else {
        std::cout << "The range contains elements less than 32.\n";
    }

    return 0;
}