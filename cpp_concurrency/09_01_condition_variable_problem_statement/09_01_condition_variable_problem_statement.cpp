#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;


std::deque<int> q;  // shared memory between threads t1 and t2
std::mutex mu;

// поставщик данных
void function_1() {
    int count = 10;

    while (count > 0) {
        // если мы не обеспечим синхронизацию доступа к очереди через мьютекс, то получим data race
        std::unique_lock<mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::this_thread::sleep_for(200ms);
        --count;
    }
}

// потребитель данных
void function_2() {
    int data = 0;

    // ОЧЕНЬ ПЛОХО, busy-waiting loop - цикл с ожиданием в состоянии занятости: поток ждёт доступа к разделяемому ресурсу,
    // периодически опрашивая флаг (data), указывающий что ресурс свободен
    while (data != 1) {  // busy-waiting loop - цикл активного ожидания освобождения ресурса
        // если мы не обеспечим синхронизацию доступа к очереди через мьютекс, то получим data race
        std::unique_lock<mutex> locker(mu);
        if (!q.empty()) {
            data = q.back();
            q.pop_back();
            locker.unlock();
            cout << "t2 got a value from t1: " << data << endl;
        } else {
            locker.unlock();
        }
    }
}

int main() {
    std::thread t1(function_1);
    std::thread t2(function_2);

    // waiting 2 threads to finish
    t1.join();  // поставщик данных
    t2.join();  // потребитель данных

    return 0;
}
