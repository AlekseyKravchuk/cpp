#include <iomanip>
#include <iostream>
#include <typeinfo>

class A {
   private:
    int x{};
};

class B : public A {
   public:
    void fB() {
        std::cout << "B::fB()" << std::endl;
    }

   private:
    int y{};
    int z{};
};

class D {
   private:
    int x_d{};
};


// создаем глобальную функцию, которая принимает в качестве параметра указатель базового типа
void F(A* pA) {
    // пытаемся сделать небезопасный UPCAST
    B* pB = static_cast<B*>(pA);
    pB->fB();
}

int main() {
    B b;
    F(&b);  // OK

    // D d;
    // F(&d);  // ошибка компилятора cannot convert ‘D*’ to ‘A*’

    A a;
    F(&a);  // скомпилируется, но может упасть в runtime

    return 0;
}
