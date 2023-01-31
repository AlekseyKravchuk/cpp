#include <iomanip>
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

int main() {
    bool isRvalue = std::is_rvalue_reference<A{20}>::value;
    std::cout << "isRvalue: "s << std::boolalpha << isRvalue << std::endl;
    return 0;
}
