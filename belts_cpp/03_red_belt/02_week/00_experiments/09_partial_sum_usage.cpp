// C++ program to demonstrate working of partial_sum()
#include <iostream>
#include <numeric>  // std::partial_sum
#include <vector>

#include "../print.h"

int main() {
    std::vector<int> v(17, 0);
    v[16] = 4;
    v[15] = 3;
    v[13] = 2;
    v[11] = 3;
    v[8] = 1;
    v[5] = 2;
    
    std::vector<int> dest(17);

    std::partial_sum(v.begin(), v.end(), dest.begin());
    std::partial_sum(v.begin(), v.end(), dest.begin());
    std::partial_sum(v.begin(), v.end(), dest.begin());

    std::cout << dest << std::endl;

    return 0;
}
