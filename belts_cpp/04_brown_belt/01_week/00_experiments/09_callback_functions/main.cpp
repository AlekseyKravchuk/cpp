#include <iostream>

int myfunc(int a) {
    return a + 1;
}

int main() {
    std::cout << "a = " << myfunc(1) << std::endl;
    return 0;
}
