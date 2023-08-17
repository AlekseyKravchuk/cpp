#include <cassert>
#include <iostream>

using namespace std;

// покроем unit-тестами функцию "Sum"
int Sum(int x, int y) {
    // if (x < 0) {
    //     x -= 1;
    // }

    return x + y /* - 1 */;
}

void TestSum() {
    // Хотим здесь собрать некоторый набор юнит-тестов, которые проверят работоспособность функции "Sum".

    assert(Sum(2, 3) == 5);
    assert(Sum(-2, -3) == -5);
    assert(Sum(-2, 0) == -2);
    assert(Sum(-2, 2) == 0);

    cout << "TestSum OK" << endl;
}

int main() {
    TestSum();

    return 0;
}
