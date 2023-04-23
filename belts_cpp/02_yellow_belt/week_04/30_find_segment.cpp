#include <iostream>
#include <utility>  // std::pair
#include <vector>

#include "00_print.h"

/*
Каким должно быть тело функции, чтобы для отсортированного диапазона целых чисел [range_begin, range_end)
она возвращала границы диапазона (полуинтервала) элементов, больших или равных left и меньших или равных right
(то есть принадлежащих отрезку [left, right])?
*/
template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindSegment(
    RandomIt range_begin, RandomIt range_end, int left, int right) {
    // ТЕЛО ФУНКЦИИ
    // return {lower_bound(range_begin, range_end, left), upper_bound(range_begin, range_end, right)};  // ERROR
    return {upper_bound(range_begin, range_end, left - 1), lower_bound(range_begin, range_end, right + 1)};
}

int main() {
    std::vector<int> v = {4, 4, 7, 7, 7, 8, 11, 15, 15, 15, 21, 23, 23, 23, 25, 26, 26};
    int left = 7;
    int right = 23;
    // int right = 24;
    auto p = FindSegment(v.begin(), v.end(), left, right);

    std::cout << "left = "s << left << ", right = "s << right << std::endl;
    std::vector<int> res{p.first, p.second};
    std::cout << res << std::endl;
    return 0;
}
