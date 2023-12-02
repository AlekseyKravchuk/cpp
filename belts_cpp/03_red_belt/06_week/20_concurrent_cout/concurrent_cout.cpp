/*
книга Яцек Галовиц "С++17 STL. Стандартная библиотека шаблонов", стр. 366 "Синхронизация и спользованияконкурентного std::cout"

Jacek Galowicz "C++17 STL Cookbook "Over 90 recipes that leverage the powerful features of the standard library in C++17", 2017,
page 425 "Synchronizing concurrent std::cout use"
*/

#include <chrono>
#include <iostream>
#include <mutex> // std::mutex, std::lock_guard
#include <sstream>
#include <thread>
#include <vector>

struct par_cout : public std::stringstream {
    static inline std::mutex _cout_mutex;

    ~par_cout() {
        std::lock_guard<std::mutex> lock{_cout_mutex};
        std::cout << rdbuf();
        std::cout.flush();
    }
};

static void print_cout(int thread_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "cout hello from thread with id = " << thread_id << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

static void print_par_cout(int thread_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    par_cout{} << "par_cout hello from thread with id = " << thread_id << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main() {
    std::vector<std::thread> v;
    size_t N = 10;

    for (size_t thr_id = 0; thr_id < N; ++thr_id) {
        v.emplace_back(print_cout, thr_id);
    }

    for (auto& thr : v) {
        thr.join();
    }

    std::cout << "=================================================" << '\n';

    v.clear();
    for (size_t thr_id = 0; thr_id < N; ++thr_id) {
        v.emplace_back(print_par_cout, thr_id);
    }

    for (auto& thr : v) {
        thr.join();
    }

    return 0;
}
