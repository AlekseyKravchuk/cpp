#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    virtual void F() /* const */ {
        std::cout << "Virtual A::F()."s << std::endl;
    }
};

class B : public A {
   private:
    void F() const override {
        std::cout << "Virtual B::F()."s << std::endl;
    }
};

int main() {
    // A* p = new B;
    // p->F();  // Virtual A::F().

    const A* p1 = new B;
    p1->F();

    return 0;
}
