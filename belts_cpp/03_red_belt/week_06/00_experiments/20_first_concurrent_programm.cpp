#include <iostream>
#include <thread>

// каждый поток должен иметь ИСХОДНУЮ ФУНКЦИЮ (initial function), с которой начинается новый поток выполнения
void hello() {
    std::cout << "Hello Concurrent World\n";
}

int main() {
    // Потоки запускаются созданием объекта std::thread.
    // Начальная функция указывается в конструкторе объекта std::thread.
    // "hello" - это начальная функция (initial function) для объекта "t"
    std::thread t(hello);  // <= Здесь запускается новый поток

    // вызов join() заставляет вызывающий поток (main()) ожидать завешения потока, связанного с объектом std::thread, в данном случае это поток "t".
    t.join();

}
