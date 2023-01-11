#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    std::vector<int> v{1, 2, 3, 2, 5, 2, 6, 2, 4, 8};
    PrintCollection(v, "Initial state of vector: ");

    v[2] = std::move(v[3]);
    PrintCollection(v, "After moving v[3] to v[2]: ");

    return 0;
}
