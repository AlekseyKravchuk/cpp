#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    A(int a) : _a(a) {}
    A() : _a(200) {}

    int Get() const {
        return _a;
    }

   private:
    int _a{};
};

std::ostream& operator<<(std::ostream& os, const A& objRef) {
    os << objRef.Get();
    return os;
}

int main() {
    A a;
    A& aLvalRef = a;
    A&& aRvalRef = A();

    // требуется создать объект типа "A"
    std::remove_reference<decltype(a)>::type x;
    static_assert(std::is_same_v<decltype(x), A>);  // OK, переменная "x" имеет тип "A"

    std::remove_reference<decltype(aLvalRef)>::type y;
    static_assert(std::is_same_v<decltype(y), A>);  // OK, переменная "y" имеет тип "A"

    std::remove_reference<decltype(aRvalRef)>::type z;
    static_assert(std::is_same_v<decltype(z), A>);  // OK, переменная "z" имеет тип "A"

    

    return 0;
}
