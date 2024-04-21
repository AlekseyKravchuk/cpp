#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <utility>  // std::move

using namespace std::chrono_literals;

const auto TIME_TO_WAIT = 5s;

// using class "thread_guard" we have the guarantee that join() will be called in the destructor
class thread_guard {
  public:
    explicit thread_guard(std::thread& t);
    explicit thread_guard(std::thread&& t);
    thread_guard(const thread_guard&) = delete;             // delete copy constructor
    thread_guard& operator=(const thread_guard&) = delete;  // delete copy assignment operator

    ~thread_guard();

  private:
    std::thread _t;  // reference to a std::thread object
};

// ================= thread_guard implementation =================
thread_guard::thread_guard(std::thread& t)
        : _t(std::move(t)) {}

thread_guard::thread_guard(std::thread&& t)
        : _t(std::move(t)) {}

thread_guard::~thread_guard() {
    if (_t.joinable()) {
        _t.join();  // we still forced to wait
//        _t.detach();
    }
}
// ============== END of thread_guard implementation =============

void foo() {
    std::cout << "Inside function foo.\n";
    std::this_thread::sleep_for(TIME_TO_WAIT);
}


void run() {
    std::thread foo_thread(foo);
    thread_guard tg(foo_thread);
}

void run2() {
    std::thread foo_thread(foo);
    thread_guard tg(std::move(foo_thread));
}

int main() {
    run();
    std::cout << "Message after run() call in main function" << std::endl;

    return 0;
}
