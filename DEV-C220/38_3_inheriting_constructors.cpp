// КАКИЕ конструкторы могут быть "inheriting constructors"

#include <utility>

class A {
   public:
    A(){};
    A(int){};
    A(const A& other) {}
    A(A&& other) {}
};

class B : public A {
    // никаких конструкторов в производном классе нет, но объекты такого класса можно создавать
    // и передавать необходимые параметры в конструктор базового класса.
    using A::A;
};

int main() {
    B b1;                  // OK
    B b2(15);              // OK
    B b3 = b2;             // OK
    B b4 = std::move(b1);  // OK

    return 0;
}
