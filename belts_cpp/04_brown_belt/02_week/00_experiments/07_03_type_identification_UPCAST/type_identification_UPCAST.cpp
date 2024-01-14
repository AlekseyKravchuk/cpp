#include <iomanip>
#include <iostream>
#include <typeinfo>

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
    // повышающее приведение типа (UPCAST) при открытом наследовании всегда безопасно
    B b;         // создаем объект производного типа
    A* pA = &b;  // заносим адрес этого объекта в указатель на базовый класс (тип)
    A& rA = b;   // сопоставляем ссылку базового типа объекту производного типа

    // A* p1 = new B;
    // std::cout << typeid(*p1).name() << std::endl;
    // std::cout << std::boolalpha << typeid(A).before((typeid(B))) << std::endl;

    return 0;
}
