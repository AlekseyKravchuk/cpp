#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

// ШАБЛОННАЯ ВЕРСИЯ PrintRange()
template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            // std::cout << *it << " "s;
            std::cout << *it << ", "s;
        } else {
            // std::cout << *it << std::endl;
            std::cout << *it << std::endl;
        }
    }
}

auto GetTupleInvalid(int x, int y) {
    return std::tie(x, y);
}

auto GetTuple(int x, int y) {
    return std::tuple(x, y);
}

auto GetVector(int x, int y) {
    return std::vector({x, y});
}

int main() {
    auto [x1, y1] = GetTuple(10, 15);
    auto [x2, y2] = GetTupleInvalid(21, 22);
    auto v = GetVector(20, 30);

    cout << "GetTuple valid (via std::tuple): " << x1 << ' ' << y1 << endl;
    cout << "GetTuple INvalid (via std::tie): " << x2 << ' ' << y2 << endl;
    std::cout << "Try with std::vector: ";
    PrintRange(v.begin(), v.end());

    return 0;
}