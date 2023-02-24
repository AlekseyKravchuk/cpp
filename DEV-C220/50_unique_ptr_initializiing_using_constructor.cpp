#include <iostream>
#include <memory>   // std::unique_ptr
#include <utility>  // std::move

class A {
   private:
    int _a{};

   public:
    A(int a) : _a(a) {}

    void SomeMethod() {
        std::cout << "SomeMethod called." << std::endl;
    }
};

int main() {
    std::unique_ptr<A> ptrA1(new A(111));

    // для объекта типа std::unique_ptr<A> вызывается перегруженный оператор "operator->()"
    ptrA1->SomeMethod();

    // для объекта типа std::unique_ptr<A> вызывается перегруженный оператор разыменования
    A a = *ptrA1;

    // // ERROR: use of deleted function; классический конструктор копирования в "std::unique_ptr" является "deleted function"
    // std::unique_ptr<A> ptrA2 = ptrA1;

    // OK: перемещающие операции для "std::unique_ptr" разрешены
    std::unique_ptr<A> ptrA3 = std::move(ptrA1);

    // // ERROR: use of deleted function; оператор присваивания в "std::unique_ptr" является "deleted function"
    // ptrA1 = ptrA3;

    // OK: ПЕРЕМЕЩАЮЩИЙ оператор присваивания в "std::unique_ptr" разрешен
    ptrA1 = std::move(ptrA3);

    return 0;
} // по этой закрывающей скобке компилятор вызовет деструкторы для всех локальных объектов
