#include <iostream>
#include <string>

using namespace std::literals;

class A {
   public:
    void fun(int) {
        std::cout << "Function \"fun\" in class A."s << std::endl;
    }
};

// Здесь работает "правило замещения имен", то есть в производном классе одноименный метод (сигнатуры методов должны быть ОДИНАКОВЫМИ)
// перекрывает собой область видимости того же самого public-метода в базовом классе
class B : public A {
   public:
    void fun(int, int) {
        std::cout << "Function \"fun\" in class B."s << std::endl;
    }
};

int main() {
    B b;
    b.fun(1, 1);  // OK

    // b.fun(1);     // error: no matching function for call to ‘B::fun(int)

    b.A::fun(1);  // OK




    return 0;
}
