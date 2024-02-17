#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

void function_1() {
    std::this_thread::sleep_for(500ms);
    cout << "Beauty is only skin-deep" << endl;
}

void do_something() {
    for (size_t i = 0; i < 100; ++i) {
        cout << R"(from "do_something": )" << i << endl;
    }
}

int main() {
    std::thread t1(function_1);  // new thread starts here, it will be associated with object "t1" (std::thread instance)

    // если "do_something" выбросит исключение, объект "std::thread" снова будет разрушен до вызова join()
    // чтобы не допустить этого, обернем потенциально опасный код в блок try-catch
    // do_something();

    try {
        do_something();
    } catch (...) {  // catch any exception that could be thrown
        // дожидаемся завершения потока
        if (t1.joinable()) {
            t1.join();
        }

        throw;
    }

    t1.join();  // main thread WAITS for "t1" to finish

    // Если объект "std::thread" будет разрушен до того, как будет joined or detached, программа завершится с ошибкой
    return 0;
}
