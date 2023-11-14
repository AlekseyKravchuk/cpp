#include <iostream>
#include <list>

#include "../../print.h"

using namespace std::literals;

int main() {
    std::list<int> lst_from = {1, 2, 3, 4, 5, 6, 7};
    std::list<int> lst_to;

    auto it_third = std::next(std::begin(lst_from), 2);
    auto it_sixth = std::next(std::begin(lst_from), 5);

    lst_to.splice(lst_to.begin(), lst_from, it_third, it_sixth);

    std::cout << lst_from << std::endl;
    std::cout << lst_to << std::endl;

    return 0;
}
