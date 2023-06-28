#include <algorithm>
#include <iostream>

using namespace std;

#define MAX(a, b) ((a > b) ? a : b)

// Если всё определение макроса не заключить в скобки, то он развернется в "int z = ((x > y) ? x : y) + 5;"
// #define MAX(a, b) (a > b) ? a : b

int main() {
    int x = 4;
    int y = 2;
    int z = MAX(x, y) + 5;

    cout << "z = "s << z << endl;

    return 0;
}
