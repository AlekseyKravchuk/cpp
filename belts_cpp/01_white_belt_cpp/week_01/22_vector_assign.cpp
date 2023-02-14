#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

#include "../my_print.h"

int main() {
    // std::vector<int> v{1, 2, 3};
    std::vector<int> v(5, 111);
    std::cout << "v.capacity() = " << v.capacity() << std::endl;
    std::cout << "v.size() = " << v.size() << std::endl;
    PrintCollection(v, "Before resize: ");

    v.resize(8, 55);
    PrintCollection(v, "AFTER resize: ");  // метод std::vector::resize оставляет нетронутой исходную часть вектора

    v.assign(3, 888);
    PrintCollection(v, "AFTER assign: ");  // метод std::vector::resize оставляет нетронутой исходную часть вектора

    return 0;
}
