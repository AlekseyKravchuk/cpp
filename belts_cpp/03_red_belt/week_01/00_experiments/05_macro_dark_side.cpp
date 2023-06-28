#include <algorithm>
#include <iostream>

using namespace std;

// аргумент макроса обязательно нужно оборачивать в скобки
#define SQR(x) ((x) * (x))

// Если всё определение макроса не заключить в скобки, то он развернется в "int z = ((x > y) ? x : y) + 5;"
// #define MAX(a, b) (a > b) ? a : b

int main() {
    int x = 3;
    int z = SQR(x + 1);
    cout << "z = "s << z << endl;

    return 0;
}
