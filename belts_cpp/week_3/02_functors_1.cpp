#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

class MyFunctor {
   public:
    // каждый раз, когда мы вызываем наш функтор, значение "count" увеличивается на 1
    void operator()() {
        std::cout << "Functor is working. Functor ID: "s << this->count << std::endl;
        ++count;
    }

   private:
    // отличительной особенностью функтора от функции является то, что он может хранить информацию о состоянии
    int count = 0;
};

template <typename T>
class Sum {
   public:
    T operator()(T lhs, T rhs) {
        return lhs + rhs;
    }

   private:
};

int main() {
    // создаем объект класса "MyFunctor"
    MyFunctor f;

    // вызываем у объекта "f" переопределенный оператор "()"
    f();  // выглядит как вызов обычной функции
    f();
    f();

    Sum<double> _sum;
    std::cout << _sum(2, 7.2) << std::endl;

    return 0;
}
