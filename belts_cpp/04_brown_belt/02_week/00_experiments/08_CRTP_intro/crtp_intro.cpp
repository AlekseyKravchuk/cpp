#include <iostream>
#include <memory> // std::shared_ptr
#include <vector>

using namespace std;

// What is static Polymorphism (CRTP)
template <typename T>
class Base {
  public:
    void f() {
        // какой тип у нас приходит в качестве параметра шаблона, к такому типу и кастуем указатель "this"
        // это позволяет нам делегировать вызов метода f() для нужного нам класса
        static_cast<T*>(this)->f();
    }
};

class Derived_1 : public Base<Derived_1> {
  public:
    void f() {
        cout << "Derived_1::f called." << endl;
    }
};

class Derived_2 : public Base<Derived_2> {
  public:
    void f() {
        cout << "Derived_2::f called." << endl;
    }
};

class Derived_3 : public Base<Derived_3> {
  public:
    void f() {
        cout << "Derived_3::f called." << endl;
    }
};

template <typename T>
void CallFunction(Base<T>& obj) {
    obj.f();
}

int main() {
    Derived_1 d_1;
    Derived_2 d_2;
    Derived_3 d_3;

    CallFunction(d_1);
    CallFunction(d_2);
    CallFunction(d_3);



    return 0;
}
