#include <vector>

/*
Задание по программированию «Разворот последовательности 2»
Реализуйте функцию vector<int> Reversed(const vector<int>& v), возвращающую копию вектора v, в которой числа переставлены в обратном порядке.
 */

std::vector<int> Reversed(const std::vector<int>& v) {
    std::vector<int> result;

    if (v.size()) {
        result.reserve(v.size());

        for (int j = (v.size() - 1); j >= 0; --j) {
            result.push_back(v[j]);
        }
    }

    return result;
}

int main() {
    // std::vector<int> numbers = {1, 5, 3, 4, 2};
    // std::vector<int> numbers = {1, 5, 3, 4};
    std::vector<int> numbers = {5};
    std::vector<int> reversed = Reversed(numbers);
    // numbers должен оказаться равен {2, 4, 3, 5, 1}
    return 0;
}
