// КАКИЕ конструкторы могут быть "inheriting constructors"

#include <utility>
#include <iostream>

using namespace std::literals;

class A {
   public:
    A(){ std::cout << "A::A()"s << std::endl; };
    A(int) {std::cout << "A::A(int)"s << std::endl; };
    A(const A& other) {std::cout << "A::A(const A& other)"s << std::endl; }
    A(A&& other) {std::cout << "A::A(A&& other)"s << std::endl; }
};

class B : public A {
   public:
    using A::A;
    // в производном классе реализованы НЕКОТОРЫЕ конструкторы
    B(int) { std::cout << "B::B(int)"s << std::endl; }
    B(int, int) { std::cout << "B::B(int, int)"s << std::endl; }
};

int main() {
    B b1;                  // A::A()
    B b2(15);              // A::A(), B::B(int)
    B b3 = b2;             // A::A(const A& other)
    B b4 = std::move(b1);  // A::A(A&& other)

    return 0;
}
