#include <iostream>
#include <vector>

using namespace std::literals;

template <typename FirstType, typename SecondType>
struct Pair {
    FirstType first{};
    SecondType second{};
};

int main() {
    Pair<std::string, int> p1;
    p1.first = "Hello"s;
    p1.second = 5;

    Pair<bool, char> p2;
    p2.first = true;
    p2.second = 'z';

    return 0;
}
