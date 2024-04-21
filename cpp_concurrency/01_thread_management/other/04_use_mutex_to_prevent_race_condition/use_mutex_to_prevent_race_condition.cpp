#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// create global mutex object
std::mutex mtx;

// Рекомендуется не использовать функции lock и unlock напрямую
void shared_print(string msg, int id) {
    // before printing the message we need to lock mutex
    mtx.lock();
    cout << msg << id << endl; // <== POTENTIAL PROBLEM: what if EXCEPTION occurs here??? ==> Mutex will be blocked forever

    // if (id >= 50) {
    //     throw std::logic_error("Intentional exception from shared_print");
    // }

    // and after printing it we need to unlock mutex
    mtx.unlock();
}

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

    try {
        std::thread t1(fun_1);
        std::thread t2(fun_2);

        t1.join();
        t2.join();
    } catch (const std::logic_error& e) {
        std::cerr << e.what() << '\n';
    }

    // std::thread t1(fun_1);
    // std::thread t2(fun_2);

    // t1.join();
    // t2.join();

    return 0;
}
