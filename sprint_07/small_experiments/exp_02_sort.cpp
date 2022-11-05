#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>  // std::make_unique
#include <string>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (next(it) != endIt) {
            cout << *it << " "s;
        } else {
            cout << *it << endl;
        }
    }
}

int main() {
    std::cout << "Unsorted:" << std::endl;
    std::vector<std::string> names = {"Tom"s, "Charlie"s, "Luna"s, "Leo"s, "Ginger"s};
    PrintRange(names.begin(), names.end());

    std::cout << "\nSorted:" << std::endl;
    std::sort(names.begin(), names.end());
    PrintRange(names.begin(), names.end());

    return 0;
}
