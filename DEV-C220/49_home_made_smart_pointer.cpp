#include <iostream>
#include <string>
#include <utility>  // std::exchange

using namespace std::literals;

template <typename T>
class smart_pointer {
   private:
    T* _raw_ptr;

   public:
    smart_pointer(T* raw_ptr) : _raw_ptr(raw_ptr) {}
    ~smart_pointer() { delete _raw_ptr; }

    smart_pointer(const smart_pointer& other) = delete;
    smart_pointer& operator=(const smart_pointer& other) = delete;

    smart_pointer(smart_pointer&& other) {
        _raw_ptr = std::exchange(other._raw_ptr, nullptr);
    }

    smart_pointer& operator=(smart_pointer&& other) {
        if (_raw_ptr != other._raw_ptr) {
            delete _raw_ptr;
            _raw_ptr = std::exchange(other._raw_ptr, nullptr);
            std::cout << "Moving assignment operator called, smart_pointer::operator=(smart_pointer&&)."s << std::endl;
        }

        return *this;
    }

    T* operator->() { return _raw_ptr; }
    T& operator*() { return *_raw_ptr; }
};

class A {
   private:
    int _a{};

   public:
    A(int a) : _a(a) {}

    void SomeMethod() {
        std::cout << "SomeMethod called." << std::endl;
    }
};

// функция "f1" принимает "smart_pointer" по значению
// когда передаем в функцию "ПО ЗНАЧЕНИЮ", компилятор обязан сформировать КОПИЮ аргумента,
// т.е. ОБЯЗАН вызвать КОНСТРУКТОР КОПИРОВАНИЯ
void f1(smart_pointer<A> p) {
    std::cout << "Function \"f1\" called." << std::endl;
}

// функция "f2" ВОЗВРАЩАЕТ "smart_pointer" по значению
smart_pointer<A> f2() {
    std::cout << "Function \"f2\" called." << std::endl;
    smart_pointer<A> tmp(new A(111));
    return tmp;
}

// функция "f3" принимает "smart_pointer" по значению
void f3(const smart_pointer<A>& ref) {
    std::cout << "Function \"f3\" called." << std::endl;
}

int main() {
    smart_pointer<A> ptrA(new A(555));
    ptrA->SomeMethod();

    smart_pointer<A> ptrB(new A(888));
    // ptrB = ptrA;  // ERROR: use of deleted function ‘smart_pointer<T>& smart_pointer<T>::operator=(const smart_pointer<T>&) [with T = A]’
    ptrB = std::move(ptrA);  // OK!

    // f1(ptrA); // ERROR: use of deleted function ‘smart_pointer<T>::smart_pointer(const smart_pointer<T>&) [with T = A]’
    f1(std::move(ptrA));  // OK!

    smart_pointer<A> ptrC = f2();  // OK

    ptrA = f2();  // ОК: вызывается перемещающий оператор присваивания


    smart_pointer<A> ptrD(new A(444));

    // в функцию "f3" отправляется АДРЕС оригинала, т.е. адрес объекта "ptrD"
    f3(ptrD);  // ОК: 


    return 0;
}
