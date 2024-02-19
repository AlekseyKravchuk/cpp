#include <future>
#include <iostream>

using namespace std;
// Bo Qian youtube channel
// https://www.youtube.com/watch?v=SZQ6-pf-5Us&list=PLxRR8W1FACnPr_prHW5Rjzh014Z8AR0dV&index=4
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
    int N = 20;

    // Вместо экземпляра класса std::thread (std::thread instance) будем использовать ФУНКЦИЮ std::async()
    // Эта функция возвращает интересную вещь - std::future<T>
    std::future<size_t> fu = std::async(std::launch::async, factorial, N);

    // fu.get() будет ждать завершения дочернего потока
    size_t res = fu.get();

    cout << "factorial(" << N << ") = " << res << endl;

    return 0;
}
