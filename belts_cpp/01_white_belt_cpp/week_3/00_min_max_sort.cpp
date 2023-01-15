#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    using namespace std::literals;

    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename Collection>
void PrintCollection(const Collection& collection) {
    using namespace std::literals;

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> v = {1, 3, 2, 5, 4};
    PrintRange(v.begin(), v.end());
    PrintCollection(v);

    std::sort(v.begin(), v.end());
    std::cout << "After sorting vector: ";
    PrintCollection(v);

    return 0;
}
