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
    // заводим объект производного типа
    // посредством этого объекта пытаемся вызвать метод базового класса "A::F()"
    // получаем ошибку
    // B b;
    // b.F();  // error: ‘virtual void B::F()’ is private within this context

    // пытаемся сделать то же самое, но через указатель на объект производного класса (указываемый объект тоже производного класса)
    // B b;
    // B* pB = &b;
    // pB->F();  // та же ошибка error: ‘virtual void B::F()’ is private within this context

    B b;
    A* pA = &b;

    // Вызывается Virtual B::F()
    pA->F();

    return 0;
}
