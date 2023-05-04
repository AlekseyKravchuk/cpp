// CPP Program to demonstrate the implementation of multimap
#include <iostream>
#include <iterator>
#include <map>

using namespace std::literals;

int main() {
    std::multimap<int, std::string> mm;
    mm.emplace(4, "aaaa"s);
    mm.emplace(2, "b"s);
    mm.emplace(6, "cd"s);
    mm.emplace(2, "aab"s);
    mm.emplace(1, "a1"s);
    mm.emplace(2, "a"s);

    for (const auto& [key, value] : mm) {
        std::cout << key << ": " << value << std::endl;
    }

    int x = 2;
    std::cout << "Equal range for x = "s << x << ": "s << std::endl;
    for (auto [it, it_range_end] = mm.equal_range(x); it != it_range_end; ++it) {
        std::cout << it->first << ": "s << it->second << std::endl;
    }

    return 0;
}
