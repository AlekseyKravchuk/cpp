#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../my_print.h"

using namespace std::literals;

int main() {
    // требуется посчитать, сколько элементов с заданным значением содержится в контейнере
    int val2count = 17;
    std::vector<int> v{8, 17, 4, 2, 11, 17, 22, 17, 7};
    PrintCollection(v, "initial state of vector: "s);

    std::cout << "value \""s << val2count << "\" occurs "s
              << std::count(v.begin(), v.end(), val2count)
              << " times in vector."s << std::endl;

    return 0;
}
