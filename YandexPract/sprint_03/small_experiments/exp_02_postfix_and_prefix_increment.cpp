#include <iostream>
#include <string>
#include <vector>

using namespace std;

void foo() {
    static int index = 0;
    cout << "Index = " << index++ <<endl;
}

int main() {
    foo();
    foo();
    foo();
    return 0;
}
