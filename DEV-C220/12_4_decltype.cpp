#include <iostream>
#include <type_traits>

int main() {
    int i = 15;
    decltype(i) n;
    // unsigned int ui = 22;

    static_assert(std::is_same_v<decltype(i), decltype(n)>);

    return 0;
}
