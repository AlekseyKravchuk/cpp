#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

using namespace std;

// нам нужно вернуть результат в "родительский" поток
size_t factorial(int N) {
    if (N == 0 || N == 1) {
        return 1;
    }

    size_t res = 1;
    for (size_t i = N; i > 1; --i) {
        res *= i;
    }

    return res;
}

int main() {
    int N = 10;

    // Вместо экземпляра класса std::thread (std::thread instance) будем использовать ФУНКЦИЮ std::async()
    // Эта функция возвращает интересную вещь - std::future<T>
    std::future<size_t> fu = std::async(std::launch::async, factorial, N);

    // fu.get() будет ждать завершения дочернего потока
    size_t res = fu.get();

    cout << "factorial(" << N << ") = " << res << endl;

    return 0;
}
