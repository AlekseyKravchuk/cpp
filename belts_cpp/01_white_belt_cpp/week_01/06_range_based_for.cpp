#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<int> v{1, 4, 6, 8, 10};
    
    int sum{};
    for (auto elm : v) {
        sum += elm;
    }

    std::cout << "sum(v{1, 4, 6, 8, 10}) = " << sum << std::endl;

    return 0;
}
