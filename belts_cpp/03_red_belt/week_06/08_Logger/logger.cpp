#include <iostream>
#include <utility>
#include <vector>

class Logger {
  public:
    Logger() {
        std::cout << "Default ctor\n";
    }

    Logger(const Logger&) {
        std::cout << "Copy ctor\n";
    }

    const Logger& operator=(const Logger& other) {
        std::cout << "Copy ASSIGNMENT operator\n";
        return other;
    }

    // // Для того, чтобы объекты класса нельзя было копировать, конструктор копирования у этого класса нужно удалить:
    // Logger(const Logger&) = delete;

    Logger(Logger&&) {
        std::cout << "Move ctor\n";
    }
};

Logger MakeLogger() {
    // здесь происходит возвращение из функции временного объекта (copy elision)
    // т.е. если в return'e временный объект (пусть даже очень тяжелый), его можно без опаски возвращать из функции

    // copy elision: возвращение из функции временного объекта
    return Logger(); // temporary -> returned (temporary)
}

Logger MakeLogger_NRVO() {
    // создаем в функции объект типа "Logger" (локальную переменную)
    Logger logger;

    // что-то делаем с объектом "logger" ...

    // Возвращаем из функции локальную переменную.
    // Это тоже безопасно с точки зрения копирования и перемещений: "Named Return Value Optimization, NRVO".
    // Здесь компилятор может обойтись и без копирований, и без перемещений.
    return logger;
}

int main() {
    Logger source;          // вызывается default-конструктор
    Logger target = source; // вызывается COPY-конструктор (конструктор копирования)

    std::cout << "-----------------------------------" << std::endl;

    std::vector<Logger> loggers;
    loggers.push_back(target); // при помещении переменной "target" внутрь вектора снова происходит вызов COPY Constructor

    std::cout << "-----------------------------------" << std::endl;

    source = target;

    /*
    // default constructor
    Logger other_logger;

    // // copy constructor: если этот конструктор в классе удален, то получаем "error: use of deleted function ‘Logger::Logger(const Logger&)’"
    // Logger logger = other_logger;

    Logger moved_logger = std::move(other_logger);  // move constructor
    */

    // // copy elision: инициализация переменной временным объектом (компилятор даже перемещать ничего не будет)
    // Logger logger = MakeLogger();

    // Logger logger = MakeLogger_NRVO();

    return 0;
}
