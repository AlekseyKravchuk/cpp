/*
Предположим, что в игре мы создаем рыцарей и магов, но нам нужно каким-то образом ограничить количество их экземпляров.
*/
#include <atomic>  // std::atomic
#include <cstddef> // size_t
#include <iostream>
#include <stdexcept> // std::logic_error

template <typename T, size_t N>
struct limited_instances {
    static std::atomic<size_t> count;

    limited_instances() {
        if (count >= N) {
            throw std::logic_error{"Too many instances"};
        }
        ++count;
    }

    ~limited_instances() {
        --count;
    }
};

template <typename T, size_t N>
std::atomic<size_t> limited_instances<T, N>::count = 0;

// Далее мы определяем производные классы
struct excalibur : public limited_instances<excalibur, 1> {};

struct book_of_magic : public limited_instances<book_of_magic, 3> {};

int main() {
    excalibur e1;
    try {
        excalibur e2;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    book_of_magic b1;
    book_of_magic b2;
    book_of_magic b3;

    try {
        book_of_magic b4;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
