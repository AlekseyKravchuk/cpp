#include <iostream>
#include <thread>
#include <chrono>

/*
 * join() method introduce a SYNCHRONIZE POINT between launched thread and thread that it launched from.
 * join() BLOCKS THE EXECUTION of the thread that calls join(), until the launched thread's execution finished.
 * join() <=> WAIT until launched thread finishes!
 *
 * detach() SEPARATES the launched thread from the thread object which it launched from.
 * detach() allows execution to continue independently.
 * detach() should not block the calling thread
 * any allocated resources inside the DETACHED thread will be freed once the thread finishes it's execution.
 * after detach() new thread will be able to perform an operation even after the parent thread has finished its execution
 */

// class to demostrate that any allocated resources will be freed once the thread exits (after detach() call)
class MyClass {
  public:
    MyClass() { std::cout << "MyClass constructor called.\n"; }
    ~MyClass() { std::cout << "MyClass DESTRUCTOR called.\n"; }
};

void func_1() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "hello from func_1 \n";
}

void func_2() {
    MyClass myclass;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "hello from func_2 \n";
}


int main() {
    std::thread thread_2(func_2);
    thread_2.detach();

    std::thread thread_1(func_1);

    thread_1.join();        // first example code. Comment this for second example
    // thread_1.detach();   // second example code. Comment this for first example

    std::cout << "hello from main thread \n";
}