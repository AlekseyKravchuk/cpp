// #include <algorithm>  // std::reverse
#include <vector>

/*
Задание по программированию «Разворот последовательности».
Реализуйте функцию void Reverse(vector<int>& v), которая переставляет элементы вектора в обратном порядке.
 */

// // 1-ый способ: через встроенный алгоритм
// void Reverse(std::vector<int>& v) {
//     std::reverse(v.begin(), v.end());
// }

// 2-й способ: с использованием std::swap
// void Reverse(std::vector<int>& v) {
//     size_t i = 0, j = v.size() - 1;
//     for (size_t k = 0; k < (v.size() / 2); ++k) {
//         std::swap(v[i++], v[j--]);
//     }
// }

// 3-й способ: hand-made
void Reverse(std::vector<int>& v) {
    if (v.size()) {
        size_t i = 0, j = v.size() - 1;
        for (size_t k = 0; k < (v.size() / 2); ++k) {
            auto tmp = v[i];
            v[i++] = v[j];
            v[j--] = tmp;
        }
    }
}

int main() {
    // std::vector<int> numbers = {1, 5, 3, 4, 2};
    // std::vector<int> numbers = {1, 5, 3, 4};
     std::vector<int> numbers = {5};
    Reverse(numbers);
    // numbers должен оказаться равен {2, 4, 3, 5, 1}
    return 0;
}
