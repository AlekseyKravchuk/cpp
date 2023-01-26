#include <iostream>
#include <string>
#include <ostream>
#include <utility>  // std::move

#include "my_print.h"

using namespace std::literals;

template <typename Type>
class ResourceManager {
   private:
    Type* _resourcePtr;

   public:
    ResourceManager(Type* resourcePtr) : _resourcePtr(resourcePtr){};

    // запрещаем конструктор копирования
    ResourceManager(const ResourceManager& other) = delete;

    // запрещаем оператор присваивания
    ResourceManager& operator=(const ResourceManager& other) = delete;

    // разрешаем перемещающий конструктор копирования
    // MOVE constructor
    ResourceManager(ResourceManager&& other) {
        // сначала забираем владение ресурсами у "other"
        this->_resourcePtr = other._resourcePtr;

        // затем обнуляем ресурсы в источнике
        other._resourcePtr = nullptr;

        std::cout << "Move constructor for ResourceManager finished." << std::endl;
    }

    // разрешаем перемещающий оператор присваивания
    ResourceManager& operator=(ResourceManager&& other) {
        if (this != &other) {
            std::swap(this->_resourcePtr, other._resourcePtr);
            delete other._resourcePtr;     // освобождаем память, на которую указывал "lhs"-указатель
            other._resourcePtr = nullptr;  // завершаем отъем ресурса у other
        }
    }

    ~ResourceManager() { delete _resourcePtr; }
};

class LifetimeLogger {
   public:
    LifetimeLogger(int id) : id_(id) {
        std::cout << "LifetimeLogger "s << id_ << " created." << std::endl;
    }

    ~LifetimeLogger() {
        std::cout << "LifetimeLogger "s << id_ << " destroyed." << std::endl;
    }

   private:
    int id_{};
};

int main() {
    // создаем LifetimeLogger в куче, передавая его конструктору "5", в качестве параметра
    LifetimeLogger* life_loggerPtr_1 = new LifetimeLogger(5);

    // далее создадим LifetimeLogger в стеке
    LifetimeLogger life_logger_2(111);

    // Из вывода программы:
    /*
    LifetimeLogger 5 created.
    LifetimeLogger 111 created.
    LifetimeLogger 111 destroyed.
     */
    // видно, что память под объект "LifetimeLogger" выделенная в куче не была освобождена, поскольку программист забыл вызвать "delete".
    // Разработаем класс ResourceManager, который позволит решить эту проблему

    // теперь создаем LifetimeLogger, используя "ResourceManager" - память больше не утекает
    ResourceManager<LifetimeLogger> life_logger_3(new LifetimeLogger(555));

    // Исследуем работу MOVE-конструктора
    ResourceManager<LifetimeLogger> life_logger_4(std::move(life_logger_3));

    return 0;
}
