#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    void General() {
        std::cout << "A::General() started."s << std::endl;
        // базовая часть работы выполняется всегда
        F();
    }

   private:
    virtual void F() {
        std::cout << "Virtual A::F() started."s << std::endl;
    }
};

class B : public A {
   private:
    virtual void F() override {
        std::cout << "Virtual B::F()."s << std::endl;
    }
};

int main() {
    A* pA = new A();
    A* pB = new B();

    // A::General() started.
    // Virtual A::F() started.
    // pA->General();

    // std::cout << std::endl;

    // 
    pB->General();

    return 0;
}
