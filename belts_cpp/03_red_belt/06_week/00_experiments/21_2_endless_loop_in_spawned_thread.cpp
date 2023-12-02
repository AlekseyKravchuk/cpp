#include <iostream>
#include <thread>

// каждый поток должен иметь ИСХОДНУЮ ФУНКЦИЮ (initial function), с которой начинается новый поток выполнения
void hello() {
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "Thread #" << this_id << ": Hello Concurrent World\n";

    // falling into endless loop:
    while (true) {

    }
}

int main() {
    // Потоки запускаются созданием объекта std::thread.
    // Начальная функция указывается в конструкторе объекта std::thread.
    // "hello" - это начальная функция (initial function) для объекта "t"
    std::thread t(hello);  // <= Здесь запускается новый поток

    // вызов join() заставляет вызывающий поток (main()) ожидать завешения потока, связанного с объектом std::thread (поток "t").
    t.join();  // <== при этом в этой точке вызывающий поток (main) БЛОКИРУЕТСЯ!!!
               // <== И нас ничто не спасет, если вдруг дочерний поток зайдет в бесконечный цикл
}
