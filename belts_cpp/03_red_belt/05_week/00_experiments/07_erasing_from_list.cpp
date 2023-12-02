#include <iostream>
#include <iterator>  // std::next
#include <list>

#include "../../print.h"

int main() {
    {
        std::list<int> numbers = {1, 2, 3, 4, 5, 6, 7};

        // хотим удалить элементы {3, 4, 5}
        std::list<int>::iterator it_from = std::next(numbers.begin(), 2);
        std::list<int>::iterator it_to = std::next(numbers.begin(), 5);
        numbers.erase(it_from, it_to);

        PrintRange(numbers.begin(), numbers.end());

        std::cout << *it_from << std::endl;
        // it_from++;  // итератор инвалидирован => EXCEPTION
        std::cout << *it_from << std::endl;
    }


    return 0;
}
