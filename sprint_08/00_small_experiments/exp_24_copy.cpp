// C++ code to demonstrate the working of std::copy

#include <algorithm>  // for copy() and copy_n()
#include <iostream>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50, 60, 70};

    std::copy(v.begin() + 3, v.begin() + 3 + 3, v.begin() + 2);

    PrintRange(v.begin(), v.end());

    return 0;
}
