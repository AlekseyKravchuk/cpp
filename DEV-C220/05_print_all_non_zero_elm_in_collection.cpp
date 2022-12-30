#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;


int main() {
    std::vector<int> v = {4, -12, -1, 3, 27};

    bool isFirst = true;
    for (auto elm : v) {
        if (elm < 0) {
            continue;
        }

        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << ", " << elm;
        }
    }
    std::cout << std::endl;

    return 0;
}
