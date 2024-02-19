#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>

using namespace std;

// we have 2 global variables that needs to be taken care of
std::mutex mu;
std::condition_variable cond_var;

// нам нужно вернуть результат в "родительский" поток
void factorial(int N, size_t& res_placeholder) {
    size_t res = 1;
    for (size_t i = N; i > 1; --i) {
        res *= i;
    }

    // "res_placeholder" is a shared variable between two threads so we need to protect it with some kind of mutex
    // При этом нам нужно быть уверенными, что переменная "res_placeholder" будет установлена в потоке "factorial" РАНЬШЕ,
    // чем родительский поток (main) воспользуется этим значением => используем механизм condition variable

    std::unique_lock<std::mutex> ulocker(mu);
    res_placeholder = res;
    ulocker.unlock();

    cond_var.notify_one();  // уведомляем ждущий поток о том, что значение факториала посчитано
}

int main() {
    size_t res_placeholder{0};
    // int N = 4;
    int N = 20;

    std::thread t1(factorial, N, std::ref(res_placeholder));

    // блокируем мьютекс
    std::unique_lock<mutex> ulocker(mu);

    // Вызов "cond_var.wait()" переведет поток #1(main) в состояние сна до тех пор, пока поток #2 (factorial) не уведомит
    // о том, что посчитал значение факториала.
    // НО!!! Перед тем, как перевести поток #1(main) в сон "ulocker" будет разблокирован, и только после этого поток #1 уснет.
    cond_var.wait(ulocker,
                  [&res_placeholder](){
                    return res_placeholder != 0;
                  });
     // Когда поступит уведомление от другого потока "ulocker" снова заблокирует мьютекс
     // и мы можем снова получим возможность безопасно (thread-safe) работать с разделяемым ресурсом (переменной "res_placeholder")
    cout << "factorial(" << N << ") = " << res_placeholder << endl;


    ulocker.unlock();

    t1.join();

    return 0;
}
