#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    A(int a);
    A() = default;
};

class B {
   public:
    B(int a) : m_AType(a) {}
    B() = default;

   private:
    A m_AType{};
};

int main() {
    B b;

    return 0;
}
