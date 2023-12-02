#include <iostream>
#include <tuple>
#include <iomanip>

int main() {
    std::tuple<int, int> t1 = {9, 26};
    std::tuple<int, int> t2 = {10, 25};

    std::cout << std::boolalpha << (t1 < t2) << std::endl;
    return 0;
}
