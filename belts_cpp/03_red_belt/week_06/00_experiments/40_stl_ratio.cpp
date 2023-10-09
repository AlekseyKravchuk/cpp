#include <chrono>
#include <future>
#include <iostream>
#include <ratio>

using namespace std::chrono;

int find_answer() {
    // Поиск ответа
    std::this_thread::sleep_for(5s); // 5 секунд
    return 42;
}

int main() {
    std::future<int> f = std::async(find_answer);

    // Ждем максимум 2.5 секунд
    if (f.wait_for(2500ms) == std::future_status::ready) {
        std::cout << "Answer is: " << f.get() << "\n";
    } else {
        std::cout << "Can't wait anymore\n";
    }

    return 0;
}
