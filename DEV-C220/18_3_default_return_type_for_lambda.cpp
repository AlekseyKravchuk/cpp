#include <algorithm>  // std::copy_if
#include <cmath>      // std::pow
#include <iostream>
#include <iterator>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    // Требуется в отдельный вектор скопировать только ОТРИЦАТЕЛЬНЫЕ значения из исходного вектора
    std::vector<int> src{5, -4, 12, 9, -11, 16, -42};

    std::vector<int> dst;
    // поскольку вектор-приемник, "dst", пустой, то нам необходим ИТЕРАТОР ВСТАВКИ "std::back_insert_iterator<Container>",
    // который любезно возвращает шаблонная функция (function template) "std::back_inserter"
    std::copy_if(src.begin(), src.end(),
                 std::back_inserter(dst),
                 [](int x) {
                    return x < 0;
                 });

    // // если бы в векторе-приемнике было бы достаточно места, то можно было бы в алгоритм "std::copy_if" передать итератор на его начало
    // // но в этом случае часть элементов вектора "dst" будут проинициализированы нулями: "-4 -11 -42 0 0 0 0"
    // std::vector<int> dst(std::size(src));
    // std::copy_if(src.begin(), src.end(),
    //              dst.begin(),
    //              [](int x) {
    //                  return x < 0;
    //              });

    // Resulting vector containing only negative values: -4 -11 -42
    PrintCollection(dst, "Resulting vector containing only negative values: "s);

    return 0;
}
