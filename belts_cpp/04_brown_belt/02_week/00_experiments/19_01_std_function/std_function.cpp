#include <iostream>

using namespace std;

template <typename T>
T sum(T x, T y) {
    return x + y;
}

template <typename T>
T subtract(T x, T y) {
    return x - y;
}


// В языке Си указатель на функцию может передаваться в другую функцию в качестве параметра.
// В функции "operation" первый параметр
// "T (*func)(T, T)" - указатель на функцию, которая принимает два параметра типа "T" и возвращает значение типа "T"
template <typename T>
int operation(T (*func)(T, T), T a, T b) {
    return func(a, b);
}

int main() {
    cout << "operation(sum, 9, 10) = " << operation(sum, 9, 10) << endl;
    cout << "operation(subtract, 9, 10) = " << operation(subtract, 9, 10) << endl;

    return 0;
}
