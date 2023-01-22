#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    virtual void F() const {
        std::cout << "Virtual A::F()."s << std::endl;
    }
};

class B : public A {
   private:
    void F() const override {
        std::cout << "Virtual B::F()."s << std::endl;
    }
};

// =================================== ФЛАГ "-fdump-lang-class" ===================================
// g++ -Wall -g -std=c++17 07_2_problem_with_virtual.cpp -fdump-lang-class -o main

// В результате использования флага "-fdump-lang-class" будет создан файл "07_2_problem_with_virtual.cpp.001l.class".
// Далее в этом файли поиском ищем "Vtable for A", где "А" - это название исследуемого класса.
// ================================================================================================

int main() {
    // A* p = new B;
    // p->F();  // Virtual A::F().

    const A* p1 = new B;
    p1->F();

    return 0;
}
