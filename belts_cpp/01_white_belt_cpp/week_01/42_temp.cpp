#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator> // std::distance

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto it = std::find(v.begin(), v.end(), 3);
    std::cout << "The number of hops between 3 and beginning of vector = " << std::distance(v.begin(), it) << std::endl;
    return 0;
}
