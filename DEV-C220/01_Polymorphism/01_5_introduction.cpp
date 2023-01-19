#include <iostream>
#include <string>
#include <utility>  // std::as_const

using namespace std::literals;

class A {
   public:
    // A() = default;
    A() {
        std::cout << "Default constructor A."s << std::endl;
    }

    A(const A&) {
        std::cout << "Copy constructor A."s << std::endl;
    }

    virtual void fun() {
        std::cout << "function \"fun\" in class A."s << std::endl;
    }
};

class B : public A {
   public:
    // B() = default;
    B() {
        std::cout << "Default constructor B."s << std::endl;
    }
    B(const B&) {
        std::cout << "Copy constructor B."s << std::endl;
    }
    virtual void fun() override {
        std::cout << "function \"fun\" in class B."s << std::endl;
    }
};

void MakeSound(A& a) {
    a.fun();
}

int main() {
    A a;
    MakeSound(a);

    std::cout << std::endl;
    
    B b;
    MakeSound(b);  // здесь компилятор вызовет конструктор копирования и отправит в "MakeSound" только копию базовой части

    return 0;
}
