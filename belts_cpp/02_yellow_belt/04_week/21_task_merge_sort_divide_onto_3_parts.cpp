#include <iostream>
#include <vector>

using namespace std;

/*
==================== Сортировка слиянием. Часть 2. Реализация с разбиением на 3 части ====================
Реализуйте сортировку слиянием, разбивая диапазон на 3 равные части, а не на 2. Гарантируется, что длина исходного диапазона является степенью 3.
Соответственно, пункты 3–5 алгоритма нужно заменить следующими:
    - Разбить вектор на 3 равные части.
    - Вызвать функцию MergeSort от каждой части вектора.
    - Слить первые две трети вектора с помощью алгоритма merge, сохранив результат во временный вектор с помощью back_inserter.
    - Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
*/

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    if (std::distance(range_begin, range_end) < 2) {
        return;
    }

    // 2. Создаем вектор, содержащий все элементы текущего диапазона.
    vector<typename RandomIt::value_type> v(range_begin, range_end);

    // 3. Разбиваем вектор на 3 равные части
    auto one_third = begin(v) + v.size() / 3;
    auto two_third = one_third + v.size() / 3;

    // 4. Вызываем функцию MergeSort от каждой трети вектора
    MergeSort(begin(v), one_third);
    MergeSort(one_third, two_third);
    MergeSort(two_third, end(v));

    // 5. Сливаем первые 2/3 вектора с помощью алгоритма merge во временный вектор с помощью "std::back_inserter"
    vector<typename RandomIt::value_type> tmp;
    merge(begin(v), one_third, one_third, two_third, back_inserter(tmp));

    // 6. Сливаем временный вектор "tmp" с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
    merge(begin(tmp), end(tmp), two_third, end(v), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}