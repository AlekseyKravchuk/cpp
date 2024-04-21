#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

std::deque<int> q;  // shared memory between threads t1 and t2
std::mutex mu;
std::condition_variable cond_var;

// поставщик данных
void function_1() {
    int count = 10;

    while (count > 0) {
        // если мы не обеспечим синхронизацию доступа к очереди через мьютекс, то получим data race
        std::unique_lock<mutex> locker(mu);
        q.push_front(count);  // добавили данные в очередь
        locker.unlock();      // разлочили мьютекс

        cond_var.notify_one();  // теперь нужно уведомить один ждущий поток, если таковой имеется
        std::this_thread::sleep_for(1s);
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

        // before the "wait()" function put thread #2 into sleep it will unlock the "locker" and then go to sleep
        // and once thread #2 is waked up by the notify from function_1, it will lock the "locker" again
        // "cond_var.wait(locker)" will put thread #2 into sleep untill being notified from thread #1
        // cond_var.wait(locker);  // but thread #2 can wake up by itself - it is called "spurious wake" (ложное пробуждение)

        // для предотвращение ложного пробуждения потока #2 нужно передать в "cond_var.wait" ещё один параметр -
        // предикат, который определит, что условие действительно выполнено, чтобы поток #2 смог продолжить своё выполнение
        // Если поток #2 проснется и обнаружит, что очередь пуста, он снова уйдет в сон.
        // Если поток #2 проснется и обнаружит, что очередь НЕ пуста, он залочит мьютекс и извлечет данные из очереди
        cond_var.wait(locker, []() {
            return !q.empty();
        });

        // после того, как поступит уведомление (notification) от другого потока, "locker" снова будет заблокирован
        // и доступ к очереди снова возобновится
        data = q.back();
        q.pop_back();
        locker.unlock();
        cout << "t2 got a value from t1: " << data << endl;
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
