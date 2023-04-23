#include <cmath>  // std::abs
#include <iostream>
#include <set>

using namespace std::literals;

/*
============== Поиск ближайшего элемента ==============

Напишите функцию FindNearestElement, для множества целых чисел numbers и данного числа border возвращающую итератор на элемент множества,
ближайший к border. Если ближайших элементов несколько, верните итератор на наименьший из них.

set<int>::const_iterator FindNearestElement(
    const set<int>& numbers,
    int border);
// set<int>::const_iterator —
// тип итераторов для константного множества целых чисел

Пример кода:

int main() {
  set<int> numbers = {1, 4, 6};
  cout <<
      *FindNearestElement(numbers, 0) << " " <<
      *FindNearestElement(numbers, 3) << " " <<
      *FindNearestElement(numbers, 5) << " " <<
      *FindNearestElement(numbers, 6) << " " <<
      *FindNearestElement(numbers, 100) << endl;
      
  set<int> empty_set;
  
  cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
  return 0;
}

Вывод:
1 4 4 6 6
1
*/

// set<int>::const_iterator — тип итераторов для константного множества целых чисел
std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border) {
    if (numbers.empty()) {
        return numbers.end();
    }

    if (numbers.count(border)) {
        return numbers.find(border);
    } else if (numbers.upper_bound(border) == numbers.begin()) {
        return numbers.begin();
    } else if (numbers.upper_bound(border) == numbers.end()) {
        return std::prev(numbers.end());
    } else {
        auto it_next = numbers.upper_bound(border);
        auto it_prev = std::prev(it_next);
        if (std::abs(border - *it_prev) < std::abs(border - *it_next)) {
            return it_prev;
        } else if (std::abs(border - *it_prev) == std::abs(border - *it_next)) {
            return (*it_prev < *it_next) ? it_prev : it_next;
        } else {
            return it_next;
        }
    }
}

int main() {
    std::set<int> numbers = {1, 4, 6};
    std::cout << *FindNearestElement(numbers, 0) << " "
              << *FindNearestElement(numbers, 3) << " "
              << *FindNearestElement(numbers, 5) << " "
              << *FindNearestElement(numbers, 6) << " "
              << *FindNearestElement(numbers, 100)
              << std::endl;

    std::set<int> empty_set;

    std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
    return 0;
}