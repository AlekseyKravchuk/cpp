#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
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
    std::vector<A> v = {A{15}, A{20}, A{35}};
    
    // либо выписываем полностью
    // std::remove_reference<decltype(*v.begin())>::type x;
    std::remove_reference_t<decltype(*v.begin())> x;

    // либо используем шаблон шаблона
    static_assert(std::is_same_v<decltype(x), A>);

    return 0;
}
