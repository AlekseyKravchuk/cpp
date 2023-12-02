#include <iostream>
#include <iterator>  // std::next
#include <vector>

using namespace std::literals;

template <typename T>
struct Widget {
    Widget(T value) : _value(value) {}
    T _value{};
};

int main() {
    Widget w_int(5);
    Widget w_char('k');
    
    return 0;
}
