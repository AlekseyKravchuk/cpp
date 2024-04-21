#include <thread>
#include <utility>

void do_something(int& i) {
    ++i;
}

struct Callable {
    int& i;

    Callable(int& val) : i(val) {}

    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j) {
            do_something(i);
        }
    }
};

// ======================= class "scoped_thread": using RAII idiom ========================
class scoped_thread {
    scoped_thread(scoped_thread const& other) = delete;       // delete copy-constructor
    scoped_thread& operator=(scoped_thread const&) = delete;  // delete copy assignment operator

   public:
    explicit scoped_thread(std::thread&& t)
        : _t(std::move(t)) {}
    ~scoped_thread();

   private:
    std::thread _t;
};

scoped_thread::~scoped_thread() {
    if (_t.joinable()) {
        // дождаться завершения потока можно, вызывав "join()" на объекте "_t"(std::thread), с которым ассоциирован этот поток
        _t.join();
    }
}
// ============================== end of class "scoped_thread" ==============================

void do_something_in_current_thread() {}

void f() {
    int local_var;
    Callable my_callable(local_var);

    // новый объект-поток (std::thread instance) вместо создания для него отдельной именованной переменной передается непосредственно в scoped_thread.
    scoped_thread guarded_thread{std::thread(Callable(local_var))};

    do_something_in_current_thread();
}

int main() {
    f();
}