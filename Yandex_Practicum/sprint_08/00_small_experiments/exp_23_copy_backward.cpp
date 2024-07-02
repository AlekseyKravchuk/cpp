// C++ code to demonstrate the working of std::copy_backward

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
    vector<int> v1 = {0, 1, 2, 3, 4};

    // Хочу сдвинуть диапазон [0, 1, 2] на 1 позицию вправо
    std::copy_backward(v1.begin(), v1.begin() + 3, v1.begin() + 4);

    // auto itStartFrom = std::find(v1.begin(), v1.end(), 3);
    // auto img_itEnd = std::find(v1.begin(), v1.end(), 0);

    // хочу сдвинуть все элементы, начиная с 3-го,  вектора "v1" на 1 позицию вправо
    // copy_backward(itStartFrom, img_itEnd , img_itEnd + 1);

    PrintRange(v1.begin(), v1.end());

    return 0;
}
