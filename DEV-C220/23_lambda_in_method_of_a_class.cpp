#include <iostream>
#include <ostream>

class C {
    int _x{0};

   public:
    C(int x) : _x(x) {}

    void f() {
        // когда лямбда используется в методе класса и нам нужен доступ к переменным объекта, то
        // что бы мы ни указали в списке захвата, "this" (адрес того объекта, для которого был вызван метод)
        // всегда сохраняется в анонимном функторе ПО ЗНАЧЕНИЮ
        // в анонимном функторе будет сохранен АДРЕС того объекта, для которого вызван метод
        [&]() { _x = 2222; }();
        // [this]() { _x = 2222; }();
        // [=]() { _x = 2222; }();
    }

    int Get() const {
        return _x;
    }
};

std::ostream& operator<<(std::ostream& os, const C& c) {
    os << c.Get() << std::endl;

    return os;
}

int main() {
    C c_obj(22);
    c_obj.f();

    std::cout << c_obj;
    
    return 0;
}
