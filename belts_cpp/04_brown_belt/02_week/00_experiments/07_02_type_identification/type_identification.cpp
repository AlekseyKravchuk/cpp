#include <iostream>
#include <typeinfo>
#include <iomanip>

class A {
   private:
    int x{};
};

class B : public A {
   private:
    int y{};
    int z{};
};

int main() {
    A* p1 = new B;
    std::cout << typeid(*p1).name() << std::endl;
    std::cout << std::boolalpha << typeid(A).before((typeid(B))) << std::endl;

    return 0;
}
