#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

template <typename RandomIt>
void Merge(RandomIt begin_it, RandomIt mid_it, RandomIt end_it) {
    auto dist = std::distance(begin_it, end_it);
    std::vector<typename RandomIt::value_type> aux(dist);

    auto left_it = begin_it;
    auto right_it = mid_it;
    auto aux_it = aux.begin();

    while (left_it < mid_it && right_it < end_it) {
        if (*left_it < *right_it) {
            *aux_it = *left_it;
            left_it = next(left_it);
        } else {
            *aux_it = *right_it;
            right_it = next(right_it);
        }

        aux_it = next(aux_it);
    }

    while (left_it < mid_it) {
        *aux_it = *left_it;
        left_it = next(left_it);
        aux_it = next(aux_it);
    }

    while (right_it < end_it) {
        *aux_it = *right_it;
        right_it = next(right_it);
        aux_it = next(aux_it);
    }

    std::copy(aux.begin(), aux.end(), begin_it);
}

template <typename RandomIt>
void MergeSort(RandomIt it_begin, RandomIt it_end) {
    auto dist = std::distance(it_begin, it_end);

    if (dist < 2) {
        return;
    }

    auto range_mid = it_begin;
    std::advance(range_mid, dist / 2);

    MergeSort(it_begin, range_mid);
    MergeSort(range_mid, it_end);
    Merge(it_begin, range_mid, it_end);
}

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

int main() {
//    vector<int> test_vector = {12, 44, 21, 35};

     vector<int> test_vector(10);

     // iota: Заполняет диапазон последовательно возрастающими значениями
     iota(test_vector.begin(), test_vector.end(), 1);

     // random_shuffle: перемешивает элементы в случайном порядке
     random_shuffle(test_vector.begin(), test_vector.end());

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    return 0;
}