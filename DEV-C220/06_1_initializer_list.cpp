#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    // bool isFirst = true;
    // for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) {
    //     if (isFirst) {
    //         std::cout << i;
    //         isFirst = false;
    //     } else {
    //         std::cout << ", "s << i;
    //     }
    // }
    // std::cout << std::endl;

    bool isFirst = true;
    for (auto i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) {
        if (isFirst) {
            std::cout << i;
            isFirst = false;
        } else {
            std::cout << ", "s << i;
        }
    }
    std::cout << std::endl;
}