/* Пример кода, который приводит к deadlock */

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1;
std::mutex mtx2;

void f1_deadlock_prone() {
    std::lock_guard<std::mutex> guard_1(mtx1);                      // Блокируем первый мьютекс mtx1
    std::this_thread::sleep_for(std::chrono::milliseconds(100));    // Засыпаем на 100 мс: имитация работы

    std::cout << "Thread 1 locked mtx1, waiting for mtx2...\n";

    std::lock_guard<std::mutex> guard_2(mtx2);                      // Просыпаемся, попытка заблокировать второй мьютекс mtx2
    std::cout << "Thread 1 acquired both locks\n";
}

void f2_deadlock_prone() {
    std::lock_guard<std::mutex> guard_2(mtx2);                       // Блокируем второй мьютекс mtx2
    std::this_thread::sleep_for(std::chrono::milliseconds(100));     // Засыпаем на 100 мс: имитация работы

    std::cout << "Thread 2 locked mtx2, waiting for mtx1...\n";

    std::lock_guard<std::mutex> guard_1(mtx1);                       // Просыпаемся, попытка заблокировать первый мьютекс mtx1
    std::cout << "Thread 2 acquired both locks\n";
}

void f1() {
    // std::lock(mtx1, mtx2) гарантирует, что оба мьютекса будут захвачены в одном порядке, избегая deadlock.
    // std::lock(mtx1, mtx2); захватывает оба мьютекса, но не освобождает их автоматически при выходе из функции.
    std::lock(mtx1, mtx2);  // Захватываем оба мьютекса атомарно

    // Теперь используем RAII для того, чтобы по выходу из области видимости РАЗБЛОКИРОВАТЬ оба мьютекса

    // Передача adopt_lock в конструктор lock_guard заставляет объект не блокировать объект мьютекса,
    // а вместо этого предполагать, что он уже заблокирован текущим потоком.
    // std::lock_guard<std::mutex> автоматически освободит мьютекс в конце области видимости.
    std::lock_guard<std::mutex> guard_1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> guard_2(mtx2, std::adopt_lock);

    std::cout << "Thread 1 acquired both locks\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));    // Засыпаем на 100 мс: имитация работы
}

void f2() {
    std::lock_guard<std::mutex> guard_1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> guard_2(mtx2, std::adopt_lock);

    std::cout << "Thread 2 acquired both locks\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));    // Засыпаем на 100 мс: имитация работы
}


int main() {
//    {
//        // deadlock
//        std::thread t1(f1_deadlock_prone);
//        std::thread t2(f2_deadlock_prone);
//        t1.join();
//        t2.join();
//    }

    {
        // avoiding deadlock
        std::thread t1(f1);
        std::thread t2(f2);
        t1.join();
        t2.join();
    }

    return 0;
}
