#include <algorithm>
#include <iostream>
#include <vector>

int main() {

    std::vector<int> data = {3, 14, 15, 201, 92, 6, 22, 75, 18, 99, 11, 2, 4, 203, 71, 4, 33, 54, 1, 18, 102, 202, 67, 21};

    std::partial_sort(data.begin(),
                      data.begin() + 3,
                      data.end(),
                      [](int lhs, int rhs) {
                        return -lhs < -rhs;
                      });

    std::cout << std::endl;

    // первыми тремя элементами в векторе будут 3, 6, 14
}