#include <iomanip>
#include <iostream>
#include <typeinfo>

class A {
   public:
    virtual void f() {
        std::cout << "A::f()" << std::endl;
    }

   private:
    int x{};
};

class B : public A {
   public:
    virtual void f() override {
        std::cout << "B::f()" << std::endl;
    }

   private:
    int y{};
    int z{};
};

class C : public B {
   public:
    virtual void f() override {
        std::cout << "C::f()" << std::endl;
    }
};

int main() {
    {
        A a;
        // здесь определение типа работает на этапе компиляции
        std::cout << typeid(a).name() << std::endl;

        B b;
        // здесь определение типа также работает на этапе компиляции
        std::cout << typeid(b).name() << std::endl;

        // ссылке на базовый тип можно сопоставить объект (его адрес) производного типа
        // указатели и ссылки - это конструкции для хранения адреса
        // ссылка хранит адрес неявно, указатель хранит адрес явно
        A& ref_a = b;
        // здесь определение типа также работает на этапе RUNTIME, т.к. ссылка - это замаскированный адрес
        // и компилятор на этапе компиляции не знает, что стоит за адресом
        std::cout << typeid(ref_a).name() << std::endl;
    }
    std::cout << "===============================================" << std::endl;

    {
        A* pA = new B();
        std::cout << typeid(pA).name() << std::endl;  // здесь компилятор укажет тип "A*"

        // совсем другое дело с разыменованным указателем
        std::cout << typeid(*pA).name() << std::endl;  // здесь компилятор укажет тип "B", на этапе RUNTIME
    }
    std::cout << "===============================================" << std::endl;

    {
        A* pA = new B;
        std::cout << "typeid(pA): " << typeid(pA).name() << std::endl;
        std::cout << "typeid(*pA): " << typeid(*pA).name() << std::endl;
        bool b = typeid(*pA) == typeid(B);
        std::cout << "typeid(*pA) == typeid(B): " << std::boolalpha << b << std::endl;

        std::cout << "typeid(*pA) == typeid(A): " << std::boolalpha << (typeid(*pA) == typeid(A)) << std::endl;
    }
    std::cout << "===============================================" << std::endl;

    {
        A* pA = new B;
        std::cout << "typeid(pA): " << typeid(pA).name() << std::endl;
        std::cout << "typeid(*pA): " << typeid(*pA).name() << std::endl;
    }
    std::cout << "===============================================" << std::endl;

    {
        B* pB = new C;
        A* pA = pB;
        std::cout << "typeid(pA): " << typeid(pA).name() << std::endl;
        std::cout << "typeid(*pA): " << typeid(*pA).name() << std::endl;
    }

    return 0;
}
