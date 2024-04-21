#include <functional> // std::bind
#include <future>
#include <mutex>      // std::once_flag, std::call_once
#include <thread>

// class A is also a functor
class A {
  public:
    void f(int x, char c) {}
    long g(double x) { return 0; }
    int operator()(int N) { return 0; }
};

int main() {
    A a;

    // Существуют 2 способа запустить новый "дочерний" поток

    // 1) непосредственно через создание объекта типа std::thread
    std::thread t1(a, 6);

    // 2) через функцию std::async
    std::future<int> fu = std::async(std::launch::async, a, 6);

    // Конструктор std::thread и функция std::async имеют сходные сигнатуры -
    // они принимают callable-object и переменное количество аргументов

    // Существуют также и другие функции со сходными сигнатурами:
    std::bind(a, 6);

    return 0;
}
