// C++ code to demonstrate the working of std::copy_backward

#include <algorithm>
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
    vector<int> v1 = {1, 2, 3, 4, 5, 6};

    // Хочу получить вектор {1, 2, 3, 1, 2, 3}

    std::copy_backward(v1.begin(), v1.begin()+3, v1.end());

    PrintRange(v1.begin(), v1.end());

    return 0;
}
