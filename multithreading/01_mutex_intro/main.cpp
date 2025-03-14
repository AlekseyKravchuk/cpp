#include <iostream>
#include <thread>

#include "mutex.h"

template<typename T>
class ThreadSafe {
  public:
    explicit ThreadSafe(T initial_value)
            : data_(initial_value) {}

    // Функция для безопасного доступа и изменения данных
    template<typename Func>
    auto access(Func&& func) {
        std::lock_guard<std::mutex> lock(mtx_);
        return func(data_);
    }

  private:
    mutable std::mutex mtx_;
    T data_ GUARDED_BY(mtx_);
};

// Функция для потока, увеличивающая значение
template<typename T>
void increment_value(ThreadSafe<T>& safe_value) {
    safe_value.access([](T& value) {
        value++;  // Увеличиваем значение
    });
}

int main() {
    // Инициализируем ThreadSafe объект значением 0
    ThreadSafe<int> safe_value(15);

    // Запускаем несколько потоков, передавая ссылку на safe_value
    std::thread t1(increment_value<int>, std::ref(safe_value));
    std::thread t2(increment_value<int>, std::ref(safe_value));
    std::thread t3(increment_value<int>, std::ref(safe_value));

    // Ожидаем завершения всех потоков
    t1.join();
    t2.join();
    t3.join();

    // Доступаемся к результату после работы всех потоков
    safe_value.access([](int& value) {
        std::cout << "Final value: " << value << std::endl;
    });

    return 0;
}
