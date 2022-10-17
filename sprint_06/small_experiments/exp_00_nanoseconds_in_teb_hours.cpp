#include <chrono>
#include <iostream>
#include <thread>

using namespace std::literals;

// Используя chrono::duration_cast, определите, сколько наносекунд в 10 часах.
int main() {
    std::cout << "В 10 часах "s << std::chrono::duration_cast<std::chrono::nanoseconds>(10h).count() << " ns"s << std::endl;

    return 0;
}
