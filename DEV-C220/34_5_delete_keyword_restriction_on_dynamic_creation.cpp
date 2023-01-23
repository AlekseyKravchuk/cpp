// Также можно наложить запрет на создание объектов в heap'e

#include <cstddef>

class A {
   public:
    // запрещаем динамическое создание для одиночного объекта
    void* operator new(std::size_t) = delete;

    // запрещаем динамическое создание для массива
    void* operator new[](std::size_t) = delete;
};

int main() {
    // Установлен запрет на динамическое создание объектов
    // A* p = new A;  // ERROR

    return 0;
}
