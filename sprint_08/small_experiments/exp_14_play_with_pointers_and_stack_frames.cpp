#include <iostream>
#include <vector>

using namespace std;

void f() {
    int a = 455;
    int b = 200;
}

int main(int argc, char const *argv[]) {
    int c = 89;
    for (int i = 0; i < 20; ++i) {
        f();
        int x = *(&c - i);
        std::cout << i << " "s << x << std::endl;
    }
    return 0;
}
