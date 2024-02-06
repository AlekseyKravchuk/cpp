#include "iterator_range.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <vector>

using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    // чтобы проитерироваться по вектору "v" у нас есть удобный механизм:
    for (const auto& elm : v) {
        cout << elm << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;

    for (int x : Head(v, 3)) {
        cout << x << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;

    // Мы можем захотеть пройтись по префиксу нашего вектора и, например, увеличить каждый его элемент на 1
    for (int& x : Head(v, 3)) {
        ++x;
    }

    // ожидаем, что первые 3 элемента вектора увеличатся на 1, а 4-ый и 5-ый останутся без изменений
    for (const auto elm : v) { // из этой записи логично ожидать, что мы меняем сам вектор
        cout << elm << ' ';
    }
    // 1 2 3  // исходный вектор не изменился
    cout << endl;

    cout << "RangeSize(Head(v, 3)) = " << RangeSize(Head(v, 3)) << endl;

    cout << "========================================" << endl;
    // Хотим работать со второй половиной вектора
    IteratorRange<vector<int>::iterator> second_half{v.begin() + v.size() / 2, v.end()};
    for (const int& x : second_half) {
        cout << x << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;
    // с помощью порождающей функции можно упростить запись
    auto second_half_neat = MakeRange(v.begin() + v.size() / 2, v.end());
    for (const int& x : second_half_neat) {
        cout << x << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;
    IteratorRange second_half_via_constuctor(v.begin() + v.size() / 2, v.end());
    for (const int& x : second_half_via_constuctor) {
        cout << x << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;
    // Now try to use template function "Head" for std::set
    set<int> numbers = {5, 7, 12, 8, 10, 5, 6, 1};

    for (int x : numbers) {
        cout << x << ' ';
    }
    cout << endl;

    for (int x : Head(numbers, 4)) {
        cout << x << ' ';
    }
    cout << endl;
    cout << "========================================" << endl;
    // Now try to use template function "Head" for std::deque
    const deque<int> nums_deque = {10, 20, 30, 40, 50, 60, 70};
    for (int x : nums_deque) {
        cout << x << ' ';
    }
    cout << endl;

    for (int x : Head(nums_deque, 4)) {
        cout << x << ' ';
    }
    cout << endl;

    return 0;
}
