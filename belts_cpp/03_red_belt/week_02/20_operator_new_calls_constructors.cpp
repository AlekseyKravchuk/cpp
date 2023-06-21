#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

struct Widget {
    Widget() {
        std::cout << "constructor of class Widget"s << std::endl;
    }

    ~Widget() {
        std::cout << "destructor of class Widget"s << std::endl;
    }
};

int main() {
    Widget* w = new Widget[3];
    delete[] w;
    return 0;
}
