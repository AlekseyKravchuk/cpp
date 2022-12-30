#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// пишем отдельный предикат для функции std::count_if
bool LethThanZero(int x) {
    return x < 0;
}

int main() {
    std::vector<int> v = {12, -3, 14, 2, -5, 7, 20};

    // нужно посчитать, сколько отрицательных элементов присутствует в коллекции
    // ptrdiff_t numOfNegative = std::count_if(v.begin(), v.end(), LethThanZero);

    // 
    ptrdiff_t numOfNegative = std::count_if(v.begin(), v.end(),
                                            [](int val) {
                                                return val < 0;
                                            });

    std::cout << numOfNegative << std::endl;

    // всё то

    return 0;
}
