#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../my_print.h"

using namespace std::literals;

int main() {
    // std::string s1 = "abcd"s;
    // std::string s2 = "abcda"s;
    // std::cout << std::min(s1, s2) << std::endl;

    std::vector<int> v{8, 17, 4, 2, 11, 7};
    PrintCollection(v, "initial state of vector: "s);

    // std::sort(v.begin(), v.end());
    // PrintCollection(v, "vector after std::sort: "s);

    std::sort(v.begin(), v.end(), [](const int lhs, const int rhs){
        return lhs > rhs;
    });

    PrintCollection(v, "vector after std::sort descending: "s);

    return 0;
}
