#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#include "temp.h"

using namespace std::literals;

int main() {
    int x = var<char>;
    int y = var<double>;
    int z = var<int>;

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;

    return 0;
}
