#include <chrono>
#include <iostream>
#include <mutex> // std::mutex, std::lock_guard
#include <thread>

using namespace std;

// create global mutex object
std::mutex mtx;


// В данном примере ресурс std::cout по-прежнему не полностью защищен от случайной модификации, т.к.
// std::cout является глобальной переменной, поэтому поток по-прежнему может писать в std::cout в обход std::lock_guard
void shared_print(string msg, int id) {
    std::lock_guard<std::mutex> guard(mtx); // RAII technique
    cout << msg << id << endl;              // <== POTENTIAL PROBLEM: what if EXCEPTION occurs here??? ==> Mutex will be blocked forever
    // if (id >= 50) {
    //     throw std::logic_error("Intentional exception from shared_print");
    // }
} // whenever the "guard" goes out of scope the mutex "mtx" be always unlocked

void fun_1() {
    for (int i = 0; i > -100; --i) {
        // cout << "From fun_1: " << i << endl;
        shared_print("From fun_1: ", i);
        // std::this_thread::sleep_for(5us);  // sleep for 5 microseconds
    }
}

void fun_2() {
    for (int i = 0; i < 100; ++i) {
        // cout << "From fun_2: " << i << endl;
        shared_print("From fun_2: ", i);
        // std::this_thread::sleep_for(3us);  // sleep for 3 microseconds
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
