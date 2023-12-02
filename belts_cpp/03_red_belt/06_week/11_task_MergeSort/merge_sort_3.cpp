#include <algorithm>
#include <memory>
#include <vector>

#include "test_runner.h"

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // Напишите реализацию функции, не копируя сортируемые элементы
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    if (std::distance(range_begin, range_end) < 2) {
        return;
    }

    // 2. Создаем вектор, содержащий все элементы текущего диапазона.
    std::vector<typename RandomIt::value_type> v(std::make_move_iterator(range_begin), make_move_iterator(range_end));

    // 3. Разбиваем вектор на 3 равные части
    auto one_third = begin(v) + v.size() / 3;
    auto two_third = one_third + v.size() / 3;

    // 4. Вызываем функцию MergeSort от каждой трети вектора
    MergeSort(std::begin(v), one_third);
    MergeSort(one_third, two_third);
    MergeSort(two_third, std::end(v));

    // 5. Сливаем первые 2/3 вектора с помощью алгоритма merge во временный вектор с помощью "std::back_inserter"
    std::vector<typename RandomIt::value_type> tmp;
    std::merge(std::make_move_iterator(std::begin(v)), std::make_move_iterator(one_third),
               std::make_move_iterator(one_third), std::make_move_iterator(two_third),
               back_inserter(tmp));

    // 6. Сливаем временный вектор "tmp" с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
    std::merge(std::make_move_iterator(std::begin(tmp)), std::make_move_iterator(std::end(tmp)),
               std::make_move_iterator(two_third), std::make_move_iterator(std::end(v)),
               range_begin);
}

void TestIntVector() {
    std::vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(std::begin(numbers), std::end(numbers));
    ASSERT(std::is_sorted(std::begin(numbers), std::end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);

    return 0;
}
