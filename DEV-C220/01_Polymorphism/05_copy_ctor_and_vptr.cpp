#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    virtual void F() {
        std::cout << "Virtual A::F()."s << std::endl;
    }
};

class B : public A {
   private:
    virtual void F() override {
        std::cout << "Virtual B::F()."s << std::endl;
    }
};

int main() {
    B b;

    // Компилятор сначала в объекте "а" (класса "А") создаст указатель на vtab класса А, 
    // и ТОЛЬКО ПОСЛЕ ЭТОГО вызовет конструктор копирования класса А: A::A(const A&),
    A a = b; // 1) A::vtab, 2) A::A(const A&)

    return 0;
}
