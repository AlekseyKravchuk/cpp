#include <cassert>
#include <iostream>
#include <thread>
#include <unistd.h> // sysconf - get configurable system variables

void __attribute__((noinline)) use(int c) { asm(""); }

int x; // область памяти

//  вариант визуальной починки гонки
// НО этот подход НЕ чинит гонку
int stick_this_thread_to_core(int core_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN); // считаем количество ядер в системе

    if (core_id < 0 || core_id >= num_cores) {
        return EINVAL;
    }

    cpu_set_t cpuset; // создаем структуру
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();

    // "pthread_setaffinity_np" распределяет "thread" на данный номер "core"
    return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void race() {
    int num = 1'000'000;
    int core_id = 0;

    stick_this_thread_to_core(core_id);

    // going UP
    for (int i = 0; i < num; ++i) {
        // std::cout << x << ' ';
        use(x);
        x += 1;
    }

    // std::cout << '\n' << "====================================================================" << std::endl;

    // going DOWN
    for (int i = 0; i < num; ++i) {
        // std::cout << x << ' ';
        use(x);
        x -= 1;
    }

    std::cout << std::endl;
}

// Гонка за область памяти (MEMORY LOCATION)
int main() {

    // каждый "thread", который входит в функцию "race" теперь перебрасывается на core #core_id,
    // в данном случае "core_id" == 0
    std::thread t1{race};
    std::thread t2{race};

    // some stuff

    t1.join(); // здесь дожидаемся окончания исполнения потока "t1"
    t2.join(); // здесь дожидаемся окончания исполнения потока "t2"

    // assert(x == 0);
    std::cout << "x = " << x << std::endl;
}
