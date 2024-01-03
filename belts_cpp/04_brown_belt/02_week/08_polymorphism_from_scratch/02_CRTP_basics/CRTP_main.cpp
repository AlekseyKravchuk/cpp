#include <iostream>

using namespace std;

template <typename T1, typename T2>
struct A {
    void func(void) {
        cout 
    }
};

// Необходимо добиться следующего эффекта:
// A<int, double> a;
// A<float, double> b;

int main() {
    A<int, double> a;
    a.func();

    return 0;
}
