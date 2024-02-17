#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

void function_1() {
    std::this_thread::sleep_for(500ms);
    cout << "Beauty is only sken-deep" << endl;
}

int main() {
    std::thread t1(function_1);  // new thread starts here, it will be associated with object "t1" (std::thread instance)

    // до того как объект "t1" выйдет из области видимости (scope), т.е. до того, как будет вызван его деструктор,
    // НЕОБХОДИМО принять решение - либо дожидаться, когда новый поток закончит своё исполнение (t1.join())
    // или сделать "t1.detach()"

    // t1.join();  // main thread WAITS for "t1" to finish

    t1.detach();  // thread associated with "t1" object now is kind of daemon (by analogy to daemon processes in Linux)

    if (t1.joinable()) {
        t1.join();
    }

    return 0;
}
