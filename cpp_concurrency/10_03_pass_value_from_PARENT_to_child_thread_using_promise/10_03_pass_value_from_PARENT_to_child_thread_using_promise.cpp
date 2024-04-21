#include <future>
#include <iostream>
#include <thread>  // std::this_thread::sleep_for
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

/*
We can pass the value from parent thread to child thread not at the time of creating the new thread,
but sometimes in the future. For that need std::async function and std::promise
*/

size_t factorial(std::future<size_t>& fu_promise) {
    size_t res = 1;

    int N = fu_promise.get();

    if (N == 0 || N == 1) {
        return 1;
    }

    
    for (size_t i = N; i > 1; --i) {
        res *= i;
    }

    return res;
}

int main() {
    std::promise<size_t> prom;  // обещание передать значение в поток когда-нибудь в будущем
    std::future<size_t> fu_prom = prom.get_future();

    // передаем экземпляр "std::future<size_t>" по ссылке в дочерний поток и говорим ему, что значение передадим позже
    // передаем по ссылке, т.к. instance of std::future<T> является НЕкопируемым
    std::future<size_t> fu = std::async(std::launch::async, factorial, std::ref(fu_prom));
    // std::this_thread::sleep_for(3s);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // в этом месте мы получили значение 
    size_t N = 10;
    prom.set_value(N);  // передаем в дочерний поток значение - сдерживаем свое обещание (promise)

    size_t res = fu.get();
    cout << "factorial(" << N << ") = " << res << endl;
    
    return 0;
}
