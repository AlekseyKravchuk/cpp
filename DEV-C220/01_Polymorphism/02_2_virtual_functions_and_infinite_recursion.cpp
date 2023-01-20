#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    virtual void Fun(int) {
        std::cout << "Virtual A::Fun(int)."s << std::endl;
    }
};

class B : public A {
   public:
    virtual void Fun(int) override {
        // В некоторых случаях явный вызов метода базового класса из метода производного класса оправдан.
        // Пусть в одноименном перегруженном методе производного класса нужно использовать метод "Fun" из базового класса.
        // Если вызов осуществить без оператора разршения области видимости, то получим рекурсию
        // Fun(20); <===== БЕСКОНЕЧНАЯ РЕКУРСИЯ!!!!!!!

        // Исправляем ошибку с бесконечной рекурсией:
        A::Fun(20);  // используем функциональность базового класса
        
        std::cout << "Virtual B::Fun(int)."s << std::endl;
    }
};

int main() {
    int x = 15;
    A* p = new B();

    // косвенный вызов через таблицу виртуальных функций
    // поскольку реальный объект типа "B" и виртуальные методы перегружены,
    // вызывается "Virtual B::Fun(int)"
    p->Fun(x);

    // явно вызываем метод базового класса
    p->A::Fun(x);

    return 0;
}
