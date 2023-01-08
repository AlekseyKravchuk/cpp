#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

template <typename T>
class A {
    T _a;

   public:
    A(const T& a) : _a(a) {}
};

auto f() {
    A ret(1);
    
    return ret;
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    
    // до C++17
    // std::pair<std::vector<int>::iterator, std::vector<int>::iterator> p(v.begin(), v.end());

    // C++17
    std::pair p(v.begin(), v.end());

    std::cout << "OK"s << std::endl;

    // до C++17
    A<int> ret_1 = f();

    // C++14
    auto ret_2 = f();

    // C++17
    // параметры шаблона можно опустить - компилятор выведет их сам по возвращаемому значению
    A ret_3 = f();

    return 0;
}
