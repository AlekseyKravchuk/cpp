#include <chrono>
#include <iostream>
#include <thread>
#include <utility>  // std::move

using namespace std;
using namespace std::chrono_literals;

// use wrapper class to implement RAII idiom around "std::thread" instance
// "std::thread::join" should be called in destructor
// =========================== ThreadWrapper ==========================
class ThreadWrapper {
   public:
    ThreadWrapper(std::thread&& t);
    ~ThreadWrapper();

   private:
    std::thread _t;
};

ThreadWrapper::ThreadWrapper(std::thread&& t)
    : _t(std::move(t)) {
}

ThreadWrapper::~ThreadWrapper() {
    if (_t.joinable()) {
        _t.join();
    }
}
// ======================= End of ThreadWrapper =======================

void function_1() {
    std::this_thread::sleep_for(500ms);
    cout << "Beauty is only skin-deep" << endl;
}

class Functor {
   public:
    void operator()() {
        for (int i = 0; i > -100; --i) {
            cout << "From thread #" << std::this_thread::get_id() << ":" << i << endl;
        }
    }
};

class FunctorGettingParameter {
   public:
    void operator()(const string& msg) {
        for (int i = 0; i > -100; --i) {
            cout << "From thread #" << std::this_thread::get_id() << ":" << msg << endl;
        }
    }
};

void do_something() {
    for (size_t i = 0; i < 100; ++i) {
        cout << R"(from "do_something": )" << i << endl;
    }
}

int main() {
    string msg_for_t2_thread = "I am t2 thread";

    ThreadWrapper(std::thread{Functor()});
    // ThreadWrapper(std::thread{FunctorGettingParameter(), std::cref(msg_for_t2_thread)});

    ThreadWrapper(std::thread{FunctorGettingParameter(), std::move(msg_for_t2_thread)});

    // Если объект "std::thread" будет разрушен до того, как будет joined or detached, программа завершится с ошибкой
    return 0;
}
