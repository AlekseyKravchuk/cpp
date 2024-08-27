#include <iostream>
#include <thread>
#include <atomic>

int main() {
    std::atomic<int> counter = 0;  // fix data race
    //int counter = 0;             // !!! Data race will occur !!!

    auto work = [&counter]() {
        for (int i = 0; i < 10'000; ++i) {
            ++counter;
        }
    };

    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();

    std::cout << counter << '\n';

    return 0;
}