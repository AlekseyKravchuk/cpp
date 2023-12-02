#include <algorithm>  // std::equal
#include <cassert>
#include <iterator>  // std::back_inserter
#include <utility>   // std::move
#include <vector>

#include "../../print.h"


int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    std::swap(v[0], v.back());
    std::cout << v << std::endl;

    return 0;
}
