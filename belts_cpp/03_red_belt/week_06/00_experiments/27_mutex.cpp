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

    mtx.lock(); // <== начиная с этого участка кода выполнение будет осуществляться в ОДНОпоточном режиме (начало КРИТИЧЕСКОЙ СЕКЦИИ)
    for (size_t row_count = 0; row_count < N; ++row_count) {
        for (size_t column_count = 0; column_count < N; ++column_count) {
            std::cout << ch;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    mtx.unlock(); // <== конец КРИТИЧЕСКОЙ СЕКЦИИ

    // эмулируем долго исполняющийся участок кода ПОСЛЕ критической секции
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // <== этот участок кода будет выполнен в многопоточном режиме
}

int main() {
    {
        LOG_DURATION("2 Print, single thread");
        Print('*');
        Print('#');
    }

    {
        LOG_DURATION("2 Print, MULTI thread");
        std::thread t1(Print, '*');
        std::thread t2(Print, '#');

        t1.join(); // в этой точке дожидаемся выполнения потока "t1"
        t2.join(); // в этой точке дожидаемся выполнения потока "t2"
    }

    return 0;
}
