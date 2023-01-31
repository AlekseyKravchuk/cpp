#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

int main() {
    int n = 1;
    const int& r = n;

    std::remove_reference<decltype(r)>::type x = n;
    static_assert(std::is_same<decltype(x), const int>::value);

    std::remove_const<decltype(r)>::type y = n;

    // тип остается без изменений, т.е. "const int&", поскольку снимается top-level константность
    static_assert(std::is_same<decltype(y), const int&>::value);

    std::remove_reference_t<std::remove_const_t<decltype(r)>> z = n;
    static_assert(std::is_same_v<decltype(z), const int>);

    std::remove_const_t<std::remove_reference_t<decltype(r)>> j = n;
    static_assert(std::is_same_v<decltype(j), int>);
    
    return 0;
}
