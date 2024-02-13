#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void fun_1() {
    for (int i = 0; i > -100; --i) {
        cout << "From fun_1: " << i << endl;
    }
}

void fun_2() {
    for (int i = 0; i < 100; ++i) {
        cout << "From fun_2: " << i << endl;
    }
}

int main() {
    // Data Race (Race Condition): здесь оба потока начинают соревноваться за общий ресурс, коим является std::cout
    // В результате вывод в std::cout является некорректным
    std::thread t1(fun_1);
    std::thread t2(fun_2);

    t1.join();
    t2.join();

    return 0;
}
