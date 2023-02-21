#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../my_print.h"

using namespace std::literals;

int main() {
    // Гораздо чаще встречаются задачи, когда нужно посчитать КОЛИЧЕСТВО элементов в контейнере, которые обладают некоторым свойством.
    // Пусть требуется посчитать, сколько элементов больших 17 содержится в контейнере
    int border = 10;
    std::vector<int> v{8, 17, 4, 2, 11, 17, 22, 17, 7};
    PrintCollection(v, "initial state of vector: "s);

    std::cout << "There are \""s << std::count_if(v.begin(), v.end(), [border](const int val) {
        return val > border;
    }) << "\" elements, that are greater than "s
              << border << "."s << std::endl;

    // Пусть требуется посчитать количество ЧЕТНЫХ элементов в контейнере
    std::cout << "There are \""s << std::count_if(v.begin(), v.end(), [](const int val) {
        return val % 2 == 0;
    }) << "\" even elements in vector."s << std::endl;

    return 0;
}
