#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class Base {
   public:
    virtual void f1() {
        std::cout << "Base::f1()."s << std::endl;
    }

    virtual void f2() {
        std::cout << "Base::f1()."s << std::endl;
    }

    virtual void f3() {
        std::cout << "Base::f1()."s << std::endl;
    }
};

class D1 : public Base {
   private:
    void f1() override {
        std::cout << "D1::f1()."s << std::endl;
    }
};

class D2 : public Base {
   private:
    void f2() override {
        std::cout << "D1::f1()."s << std::endl;
    }
};

// =================================== ФЛАГ "-fdump-lang-class" ===================================
// g++ -Wall -g -std=c++17 07_2_problem_with_virtual.cpp -fdump-lang-class -o main

// В результате использования флага "-fdump-lang-class" будет создан файл "07_2_problem_with_virtual.cpp.001l.class".
// Далее в этом файли поиском ищем "Vtable for A", где "А" - это название исследуемого класса.
// ================================================================================================

// ================== Таблицы виртуальных функций для классов ==================
/* 
Vtable for Base
Base::_ZTV4Base: 5 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI4Base)
16    (int (*)(...))Base::f1
24    (int (*)(...))Base::f2
32    (int (*)(...))Base::f3

Vtable for D1
D1::_ZTV2D1: 5 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI2D1)
16    (int (*)(...))D1::f1    // <== метод "f1" в производном классе "D1" ПЕРЕОПРЕДЕЛЕН
24    (int (*)(...))Base::f2  // метод "f2" в производном классе "D1" НЕ переопределен,=> по данному смещению размещен адрес метода базового класса, Base::f2
32    (int (*)(...))Base::f3  // метод "f3" в производном классе "D1" НЕ переопределен,=> по данному смещению размещен адрес метода базового класса, Base::f3

Vtable for D2
D2::_ZTV2D2: 5 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI2D2)
16    (int (*)(...))Base::f1   // метод "f1" в производном классе "D2" НЕ переопределен,=> по данному смещению размещен адрес метода базового класса, Base::f1
24    (int (*)(...))D2::f2     // <== метод "f2" в производном классе "D1" ПЕРЕОПРЕДЕЛЕН
32    (int (*)(...))Base::f3   // метод "f3" в производном классе "D2" НЕ переопределен,=> по данному смещению размещен адрес метода базового класса, Base::f3
 */
// ==============================================================================

int main() {
    // создаем экземпляр класса "D1"
    D1 d1;

    // создаем указатель на базовый класа посредством адреса объекта производного класса "D1"
    Base *dPtr = &d1;

    dPtr->f1();
    dPtr->f2();
    dPtr->f3();

    return 0;
}
