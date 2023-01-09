// Требуется, используя "std::copy_if" и лямбда-функцию посчитать количество скопированных элементов в заданном диапазоне,
// удовлетворяющих заданному критерию (четные элементы)

#include <algorithm>  // std::copy_if
#include <iostream>
#include <iterator>  // std::begin(), std::end(), std::back_inserter
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    std::vector<int> source = {23, 2, 11, 7, 12, 35, 17, 8, 19, 13, 49};
    std::vector<int> destination;

    int lower{2}, upper{14}, count{0};
    std::copy_if(source.begin(), source.end(), std::back_inserter(destination),
                 [lower, upper, &count](int val) -> bool {
                     if (val >= lower && val < upper && val % 2 == 0) {
                         ++count;
                         return true;
                     } else {
                         return false;
                     }
                 });

    PrintCollection(destination);

    std::cout << "Copied " << count << " elements." << std::endl;

    return 0;
}
