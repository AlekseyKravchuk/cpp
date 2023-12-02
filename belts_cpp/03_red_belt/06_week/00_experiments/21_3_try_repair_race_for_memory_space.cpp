#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

int x;               // область памяти
std::mutex mu_for_x; // мьютекс для синхронизации x

void race() {
    int num = 1'000'000;

    // going UP
    for (int i = 0; i < num; ++i) {
        // std::cout << x << ' ';
        mu_for_x.lock();
        x += 1;
        mu_for_x.unlock();
    }

    // going DOWN
    for (int i = 0; i < num; ++i) {
        // std::cout << x << ' ';
        mu_for_x.lock();
        x -= 1;
        mu_for_x.unlock();
    }
}

// Гонка за область памяти (MEMORY LOCATION) теперь УСТРАНЕНА за счет использования "std::mutex"
int main() {

    std::thread t1{race};
    std::thread t2{race};

    // some stuff

    t1.join(); // здесь дожидаемся окончания исполнения потока "t1"
    t2.join(); // здесь дожидаемся окончания исполнения потока "t2"

    assert(x == 0);
    std::cout << "x = " << x << std::endl;
}
