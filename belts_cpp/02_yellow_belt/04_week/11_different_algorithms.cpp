#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<int> vec = {7, 8, 12, 17, 5, 4, 32, 18};
    auto it = std::is_sorted_until(vec.begin(), vec.end());
    auto it_rev = std::is_sorted_until(vec.rbegin(), vec.rend());

    std::cout << *it << std::endl;
    std::cout << *it_rev << std::endl;
    return 0;
}
