#include <algorithm>
#include <iostream>
#include <vector>

#include "../../print.h"

int main() {
    std::vector<int> v = {222};
    std::swap(v[0], v[0]);

    std::cout << v << std::endl;

    return 0;
}
