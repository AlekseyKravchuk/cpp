#include <iostream>

#include "sum_reverse_sort.h"
#include "pretty_print.h"

using namespace std::literals;

int main() {
    std::cout << Sum(5,7) << std::endl;
    std::cout << Reverse("abcd"s) << std::endl;

    std::vector<int> v = {3, 8, 1, 14, 5, 6};
    Sort(v);
    std::cout << v << std::endl;
    return 0;
}
