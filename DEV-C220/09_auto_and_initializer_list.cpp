#include <iostream>
#include <string>

int main() {
    auto x5 = {1, 2, 3};
    
    // а так - ОШИБКА
    // auto x5{1, 2, 3};  // ERROR: direct-list-initialization of ‘auto’ requires exactly one element

    for (auto elm : x5) {
        std::cout << elm << std::endl;
    }

    return 0;
}
