#include <cassert>
#include <iostream>

using namespace std;

int Sum(int x, int y) {
    return x + y;
}

void TestSum() {
    assert(Sum(2, 3) == 5);
    assert(Sum(-2, -3) == -5);
    assert(Sum(-2, 0) == -2);
    assert(Sum(-2, 2) == 0);

    std::cout << "TestSum OK" << std::endl;
}

int main() {
    TestSum();

    return 0;
}
