#include <iostream>
#include <map>
#include <string>

#include "../print.h"

using namespace std::literals;

int main() {
    std::map<int, std::string> m = {
        {10, "ten"s},
        {12, "twelve"s},
        {15, "fifteen"s},
        {19, "nineteen"s},
        {22, "twenty two"s},
        {25, "twenty five"s},
    };

    int time = 26;
    const int TIME_AGO = 7;
    auto it = m.upper_bound(time - TIME_AGO);

    m.erase(m.begin(), it);

    std::cout << m << std::endl;

    return 0;
}
