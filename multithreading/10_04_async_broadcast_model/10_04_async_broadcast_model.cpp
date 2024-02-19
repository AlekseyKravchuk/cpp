#include <array>
#include <chrono>
#include <future>
#include <iostream>
#include <thread> // std::this_thread::sleep_for

using namespace std;
using namespace std::chrono_literals;


/*
We can pass the value from parent thread to child thread not at the time of creating the new thread,
but sometimes in the future. For that need std::async function and std::promise
*/

size_t factorial(std::shared_future<size_t> sf) {
    size_t res = 1;

    int N = sf.get(); // на каждой ОБЫЧНОЕ (не-shared) фьюче можно вызвать метод "get()" ТОЛЬКО 1 раз

    if (N == 0 || N == 1) {
        return 1;
    }

    for (size_t i = N; i > 1; --i) {
        res *= i;
    }

    return res;
}

int main() {
    std::promise<size_t> prom; // обещание передать значение в поток когда-нибудь в будущем
    std::future<size_t> fu_prom = prom.get_future();

    // // Предположим, что нам нужно запустить 5 потоков для вычисления факториала с одним и тем же значением
    // // ПРОБЛЕМА: мы не можем передать экземпляр "std::future" во все 5 потоков, т.к. фьюча является move-only
    // std::future<size_t> fu1 = std::async(std::launch::async, factorial, std::ref(fu_prom));
    // std::future<size_t> fu2 = std::async(std::launch::async, factorial, std::ref(fu_prom));
    // std::future<size_t> fu3 = std::async(std::launch::async, factorial, std::ref(fu_prom));
    // std::future<size_t> fu4 = std::async(std::launch::async, factorial, std::ref(fu_prom));
    // std::future<size_t> fu5 = std::async(std::launch::async, factorial, std::ref(fu_prom));

    // РЕШЕНИЕ
    std::shared_future<size_t> sh_fu = fu_prom.share();

    // В отличие от обычной фьючи, "std::shared_future<T>" может быть скопирована
    // ==> передаем в дочерние потоки "std::shared_future<T>" по значению
    std::future<size_t> fu1 = std::async(std::launch::async, factorial, sh_fu);
    std::future<size_t> fu2 = std::async(std::launch::async, factorial, sh_fu);
    std::future<size_t> fu3 = std::async(std::launch::async, factorial, sh_fu);
    std::future<size_t> fu4 = std::async(std::launch::async, factorial, sh_fu);
    std::future<size_t> fu5 = std::async(std::launch::async, factorial, sh_fu);

    // std::this_thread::sleep_for(3s);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // в этом месте мы получили значение
    size_t N = 10;

    // Когда родительский поток (main) установит значение "N", все 5 потоков получат это значение.
    // "std::shared_future<T>" удобна, когда у нас широковещательная модель взаимодействия между потоками
    prom.set_value(N); // передаем в дочерний поток значение - сдерживаем свое обещание (promise)

    std::array<std::future<size_t>, 5> futures = {move(fu1), move(fu2), move(fu3), move(fu4), move(fu5)};
    std::array<size_t, 5> results;

    for (size_t i = 0; i < 5; ++i) {
        results[i] = futures[i].get();
    }

    for (size_t i = 0; i < 5; ++i) {
         cout << "factorial(" << N << ") = " << results[i]
              << "(from future #" << (i+1) << ")" << endl;
    }


    return 0;
}
