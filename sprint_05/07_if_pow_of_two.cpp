#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

bool IsPowOfTwo(int n) {
    if (n == 1) {
        return true;
    }

    if ((n <= 0) || (n % 2 != 0)) {
        return false;
    }

    return IsPowOfTwo(n / 2);
}

int main() {
    int n;
    cout << "Type in number: ";
    cin >> n;
    bool result = IsPowOfTwo(n);
    cout << boolalpha << result << endl;
}
