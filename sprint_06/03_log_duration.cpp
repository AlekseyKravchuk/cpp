#include <chrono>
#include <cstdlib>  // std::rand
#include <iostream>
#include <thread>   // std::this_thread
#include <vector>

using namespace std::literals;

class LogDuration {
   public:
    LogDuration() {
    }

    ~LogDuration() {
        const std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
        const std::chrono::steady_clock::duration dur = endTime - _startTime;
        std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms"s << std::endl;
    }

   private:
    // В переменной будет время конструирования объекта LogDuration
    const std::chrono::steady_clock::time_point _startTime = std::chrono::steady_clock::now();
};

int main() {
    std::cout << "Ожидание 5s..."s << std::endl;

    {
        LogDuration sleepGuard;
        // операция - ожидание 5 секунд
        std::this_thread::sleep_for(5s);
    }

    std::cout << "Ожидание завершено"s << std::endl;
}
