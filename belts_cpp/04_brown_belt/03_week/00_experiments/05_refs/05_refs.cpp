#include <iostream>
#include <map>
#include <set>

using namespace std;

int main() {
    int var = 15;
    int& ref = var;
    int a = ref;
    a += 200;

    cout << "a = " << a << endl;
    cout << "ref = " << ref << endl;

    return 0;
}
