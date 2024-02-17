#include <iostream>
#include <mutex>

std::once_flag flag;

void do_something() {
    std::call_once(flag,
                   []() {
                       std::cout << "Happens once" << std::endl;
                   });

    std::cout << "Happens every time" << std::endl;
}

int main() {
    for (int i = 0; i < 10; ++i) {
        do_something();
    }
    return 0;
}
