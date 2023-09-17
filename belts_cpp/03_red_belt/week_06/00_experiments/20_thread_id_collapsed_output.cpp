#include <iostream>
#include <thread>

// Что-то похожее на гонку за std::cout.
// Вывод от родительского потока и дочернего перемешиваются.
int main() {
    std::thread child_thread([]() {
        std::cout << "Hello Concurrent World!\n";
        std::cout << "child(spawned): " << std::this_thread::get_id() << '\n';
    }); // в момент завершения конструктора поток запустился и начал работать

    // делаем что-то в основном потоке (у нас всегда есть хотя бы 1 поток), пока исполняется поток "child_thread"
    std::cout << "main: " << std::this_thread::get_id() << '\n';

    child_thread.join(); // в этой точке (вызов "join") мы дожидаемся исполнения потока "child_thread"
                         // после того, как мы дождались завершения потока, поток больше не существует
}
