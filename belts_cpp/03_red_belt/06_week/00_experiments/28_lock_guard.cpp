#include <chrono>
#include <iostream>
#include <mutex> // std::mutex, std::lock_guard
#include <thread>

#include "../../log_duration.h"

// мьютек нам нужен, чтобы синхронизировать потоки и защитить разделяемые данные (участок кода), чтобы некоторая последовательность действий
// выполнялась строго в однопоточном режиме, чтобы остальные потоки ждали завершения работы над критической секцией
std::mutex mtx; // глобальный объект класса "mutex"

void Print(const char ch) {
    size_t N = 5;

    // эмулируем долго исполняющийся участок кода ДО критической секции
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // <== этот участок кода будет выполнен в многопоточном режиме

    {
        // задача "lock_guard" - хахватить мьютекс в конструкторе (вызов "mtx.lock()") и освободить его в деструкторе (вызов "mtx.unlock()")
        // передаем в КОНСТРУКТОР "std::lock_guard<std::mutex>" мьютекс, который мы ранее создавали
        std::lock_guard<std::mutex> guard(mtx);

        for (size_t row_count = 0; row_count < N; ++row_count) {
            for (size_t column_count = 0; column_count < 2 * N; ++column_count) {
                std::cout << ch;
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    } // здесь будет вызван деструктор "std::lock_guard<std::mutex>" и будет вызван метод "mtx.unlock()"

    // эмулируем долго исполняющийся участок кода ПОСЛЕ критической секции
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // <== этот участок кода будет выполнен в многопоточном режиме
}

int main() {
    {
        LOG_DURATION("3 Print, single thread");
        Print('*');
        Print('#');
        Print('=');
    }

    {
        LOG_DURATION("3 Print, MULTI thread");
        std::thread t1(Print, '*');
        std::thread t2(Print, '#');
        std::thread t3(Print, '=');

        t1.join(); // в этой точке дожидаемся выполнения потока "t1"
        t2.join(); // в этой точке дожидаемся выполнения потока "t2"
        t3.join(); // в этой точке дожидаемся выполнения потока "t3"
    }

    return 0;
}
