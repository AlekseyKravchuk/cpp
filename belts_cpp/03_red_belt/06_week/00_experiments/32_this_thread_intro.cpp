/*
Jacek Galowicz - C++17 STL Cookbook (2017)
Chapter 9. Putting a program to sleep for specific amounts of time
*/

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    // "std::this_thread::sleep_for" блокирует выполнение текущего потока на конкретный промежуток времени.
    // Заблокированный поток не потребляет время процессора на протяжении блокировки.
    // "std::this_thread::sleep_for" принимает значение типа "chrono::duration".
    std::cout << "Going to sleep for 3 seconds and 300 milliseconds.\n";
    std::this_thread::sleep_for(3s + 300ms);

    // "std::this_thread::sleep_until" также блокирует выполнение текущего потока на конкретный промежуток времени.
    // Заблокированный поток не потребляет время процессора на протяжении блокировки.
    // "std::this_thread::sleep_until" принимает значение типа "chrono::time_point".
    // Это удобно в том случае, если нужно приостановить поток до наступления конкретного момента времени.
    // Точность момента пробуждения зависит от операционной системы. Она будет довольно высокой для большинства ОС,
    // но могут возникнуть проблемы, если требуется точность вплоть до наносекунд.
    std::cout << "Going to sleep for another 2 seconds.\n";
    std::this_thread::sleep_until(std::chrono::high_resolution_clock::now() + 2s);

    return 0;
}
