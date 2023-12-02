#include <cassert>
#include <iostream>
#include <mutex> // std::mutex, std::lock_guard
#include <thread>

int x;               // область памяти
std::mutex mu_for_x; // мьютекс для синхронизации x

void race() {
    int num = 1'000'000;

    // going UP
    for (int i = 0; i < num; ++i) {
        // Защелка "std::lock_guard<T>" - это RAI-обертка над любым классом,
        // поддерживающим интерфейс из методов "lock" и "unlock"

        std::lock_guard<std::mutex> lk{mu_for_x};
        x += 1;
    }  // до вызова деструктора имеем критическую секцию за счет использования "std::lock_guard<std::mutex>"

    // going DOWN
    for (int i = 0; i < num; ++i) {
        std::lock_guard<std::mutex> lk{mu_for_x};
        x -= 1;
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
