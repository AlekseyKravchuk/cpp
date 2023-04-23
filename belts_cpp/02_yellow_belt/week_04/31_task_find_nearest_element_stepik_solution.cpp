#include <iostream>
#include <iterator>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    const auto first_equal_or_greater = numbers.lower_bound(border);

    // Если множество пусто или элемент "border" МЕНЬШЕ или РАВЕН первому элементу во множестве
    if (first_equal_or_greater == numbers.begin()) {
        return first_equal_or_greater;
    }

    // если оказались здесь, то гарантированно в множестве присутствует ХОТЯ БЫ 1 элемент и "border" != первому элементу множества
    const auto it_prev = prev(first_equal_or_greater);

    // Если элементы, РАВНЫЕ или БОЛЬШИЕ "border" ОТСУТСТВУЮТ, то "lower_bound" возвращает "numbers.end()"
    if (first_equal_or_greater == numbers.end()) {
        return it_prev;
    }

    auto diff1 = border - *it_prev;
    auto diff2 = *first_equal_or_greater - border;
    
    return (diff1 <= diff2) ? it_prev : first_equal_or_greater;
}

int main() {
    // set<int> numbers = {1, 4, 6};
    // auto it = FindNearestElement(numbers, 3);

    set<int> numbers = {1, 5, 6};
    auto it = FindNearestElement(numbers, 3);
    std::cout << *it << std::endl;
    return 0;
}
