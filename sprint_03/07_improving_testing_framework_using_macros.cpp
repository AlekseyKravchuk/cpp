#include <iostream>

using namespace std;

// #<имя параметра> подставляет строковое представление параметра макроса
#define PrintExpression(expr) cout << (expr) << " = "s << (expr) << endl

int main() {
    PrintExpression((8 + 15) * 3);
}
