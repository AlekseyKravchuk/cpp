#include <iostream>
#include <ostream>

class A {
    int _a;

   public:
    A(int a) : _a(a) {}
    int GetA() { return _a; };
};

class B {
    int _b;

   public:
    explicit B(int b) : _b(b) {}
    int GetB() { return _b; };
};

std::ostream& operator<<(std::ostream& os, A& inst_a) {
    os << inst_a.GetA();
    return os;
}

std::ostream& operator<<(std::ostream& os, B& inst_b) {
    os << inst_b.GetB();
    return os;
}

int main() {
    A a1{1};     // так можно
    A a2 = {2};  // и так можно

    B b1{1};     // так можно
    // B b2 = {2};  // А так уже НЕЛЬЗЯ потому что для класса B конструктор - explicit

    std::cout << a1 << " " << a2 << std::endl;

    // std::cout << b1 << " " << b2 << std::endl;
    std::cout << b1 << std::endl;

    return 0;
}
