#include <functional> // std::bind
#include <future>
#include <mutex> // std::once_flag, std::call_once
#include <thread>

// class A is also a functor
class A {
  public:
    void f(int x, char c) {}
    long g(double x) { return 0; }
    int operator()(int N) { return 0; }
};

void foo(int x) { }

int main() {
    A a;
    std::thread t1(a, 6); // copy of a() and then invoke it in a different thread

    // если мы хотим избежать копирования объекта типа "A"
    std::thread t2(std::ref(a), 6); // launch a() in different thread

    // с использованием перемещения:
    std::thread t3_1(A{}, 6); // temp A

    // или так:
    std::thread t3_2(std::move(a), 6); // 'a' is no longer usable in main thread

    // также мы можем передать лямбда-функцию в конструктор std::thread
    std::thread t4([](int x) { return x*x; }, 6);

    // или обычную функцию
    std::thread t5(foo, 7);

    // мы также можем передать метод класса в конструктор std::thread
    std::thread t6(&A::f, a, 20, 'w');  // copy of a.f(8, 'w) in a different thread

    std::thread t7(&A::f, std::ref(a), 20, 'w');  // copy of a.f(8, 'w) in a different thread

    return 0;
}
