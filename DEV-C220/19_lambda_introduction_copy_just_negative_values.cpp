// скопировать с помощью std::copy_if только НЕОТРИЦАТЕЛЬНЫЕ значения из массива в вектор

#include <algorithm>  // std::copy_if
#include <iterator>   // std::begin(), std::end(), std::back_inserter
#include <vector>

#include "my_print.h"

int main() {
    int ar[] = {5, 9, -6, -10, 20};
    std::vector<int> v;

    // шаблонная функция "std::back_inserter" формирует итератор вставки "std::back_insert_iterator"
    std::copy_if(std::begin(ar), std::end(ar), std::back_inserter(v),
                 [](int val) {
                    return val > 0;
                 });

    PrintCollection(v);

    return 0;
}
