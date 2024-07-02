#include <iostream>
#include <string>

using namespace std;

uint64_t Factorial(int n) {
    int factorial = 1;

    while (n > 1) {
        factorial *= n;
        --n;
    }

    return factorial;
}

uint64_t FactorialRecursive(int n) {
    if (n == 0) {
        return 1;
    }

    int factorial = n * FactorialRecursive(n-1);

    return factorial;
}

int main() {
    int n;
    cout << "Type in number to calculate factorial: ";
    cin >> n;

    // cout << "factorial(" << n << ") = " << Factorial(n) << endl;
    cout << "factorial(" << n << ") = " << FactorialRecursive(n) << endl;

    return 0;
}
