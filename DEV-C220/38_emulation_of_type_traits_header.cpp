#include <iostream>
#include <iterator>  // std::move_iterator, std::make_move_iterator
#include <ostream>
#include <set>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "my_print.h"

using namespace std::literals;

struct is_int {
    bool value;
};

int main() {
    int n = 1;

    if (is_int{typeid(int) == typeid(n)}.value) {
        std::cout << "int"s << std::endl;
    }

    return 0;
}
