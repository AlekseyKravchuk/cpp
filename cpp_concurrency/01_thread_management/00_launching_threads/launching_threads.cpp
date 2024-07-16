#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
const auto TIME_TO_WAIT = 2s;

void func1() {
    std::thread::id this_id = std::this_thread::get_id();
    std::this_thread::sleep_for(TIME_TO_WAIT);
    std::cout << "Hello from ordinary function \"func1\". Thread ID = " << this_id << "\n";
}

class CallableClass {

  public:
    void operator()() {
        std::thread::id this_id = std::this_thread::get_id();
        std::this_thread::sleep_for(TIME_TO_WAIT);
        std::cout << "hello from the class with function call operator. Thread ID = " << this_id << "\n";
    }
};

void run() {
    std::thread::id this_id = std::this_thread::get_id();
    std::this_thread::sleep_for(TIME_TO_WAIT);
    std::cout << "This message is from main thread. Thread ID = " << this_id << "\n";
}

/*
 * Для запуска потока мы должны передать "callable object" в конструктор std::thread.
 * В качестве такого "callable object" могут выступать
 *    - адрес обычной функции (или просто ее имя);
 *    - объект функтора (т.е. класса, в котором определен опрератор operator());
 *    - lambda expression.
 * После запуска потока в каком-то месте нашего "родительского" потока мы должны выполнить "joining" (а по сути - "waiting"),
 * т.е. посредством объекта класса std::thread, с помощью которого был запущен дочерний поток, вызвать "join()".
 * Метод "join" заставит родительский поток ЖДАТЬ, пока поток, который он вызвал (через создание объекта "std::thread"),
 * завершит свое выполнение.
 */
int main() {
    //create thread using fuction
    std::thread thread1(&func1);

    //create thread using class with function call operator
    CallableClass callable;
    std::thread thread2(callable);

    //create thread using lambda expression
    std::thread thread3([]() {
        std::thread::id this_id = std::this_thread::get_id();
        std::this_thread::sleep_for(TIME_TO_WAIT);
        std::cout << "hello from the lambda.  Thread ID = " << this_id << "\n";
    });


    thread1.join();  // WAIT until "thread1" finishes it's execution
    thread2.join();  // WAIT until "thread2" finishes it's execution
    thread3.join();  // WAIT until "thread3" finishes it's execution

    run();
}
