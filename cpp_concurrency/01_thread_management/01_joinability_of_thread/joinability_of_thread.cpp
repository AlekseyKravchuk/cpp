#include <iostream>
#include <thread>

/*
 * Properly constructed thread object (std::thread instance) represents an active thread of execution in hardware level.
 * To construct a thread properly, we have to pass CALLABLE object as an argument to the std::thread class constructor.
 * If this callable takes parameters, we have to pass those parameters properly as well.
 * Such a thread object is JOINABLE.
 * --> For any "joinable" thread, we must CALL either 'join' or 'detach' method.
 * --> After we made such a call ('join' or 'detach') that thread becomes NON JOINABLE.
 *
 * If you forgot to join() or detach() on joinable thread, then at the time of destructor call to that thread object,
 * "std::terminate" function will be called.
 *
 * If any program have "std::terminate" call we refer such a program as UNSAFE PROGRAM.
 */

void func_1() {
    std::cout << "hello from method \n";
}


int main() {
    std::thread thread_1(func_1);
    std::thread test_thread;

    // we can check whether the particular thread is joinable or not using "joinable()" method for particular thread
    if (thread_1.joinable())
        std::cout << "this is joinable thread \n";

    // we HAVE TO call join() or detach() on "thread_1" somewhere in our program before "thread_1" destructor call.
    thread_1.join();

    if (thread_1.joinable()) {
        std::cout << "this is joinable thread \n";
    } else {
        std::cout << "after calling join, thread_1 is not a joinable thread \n";
    }

    // After either call to join() or detach() method that thread becomes NON JOINABLE!
    // For all JOINABLE thread in our program we have to call either join() or detach().
    // join() or detach() call to NON JOINABLE thread would result in an exception.

    std::cout << "hello from main thread \n";

    std::cout << "thread \"test_thread\" ";
    std::cout << (test_thread.joinable()
                  ? "is joinable"
                  : "is NOT joinable") << std::endl;
}