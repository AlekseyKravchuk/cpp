#include <fstream>
#include <string>
#include <utility>

std::pair<std::ifstream, std::ofstream> MakeStreams(const std::string& prefix) {
    std::ifstream input(prefix + ".in");
    std::ofstream output(prefix + ".out");

    // 1) передача переменных "input", "output" в коструктор пары происходит по обычным правилам языка С++:
    //    это не временные объекты и поэтому они пытаются скопироваться в конструктор пары => сообщение об ошибке
    // 2) а затем созданная с помощью этого конструктора пара должна проинициализировать возвращаемый временный объект
    // 3) поскольку эта пара тоже временная, то инициализация временного объекта, возвращаемого из функции этой парой,
    //    происходит безболезнено - срабатывает copy elision
    // return {input, output};

    // return {std::move(input), std::move(output)};
    return {std::ifstream(prefix + ".in"), std::ofstream(prefix + ".out")};
}

std::ifstream MakeInputStream(const std::string& prefix) {
    // auto streams = MakeStreams(prefix);

    // // хотим вернуть только 1 поле локальной переменной
    // // "streams" - это некоторая локальная переменная, "streams" - это некоторое её поле
    // // это не временный объект, значит "copy elision" не сработает
    // // Named Return Value Optimization (NRVO) тоже не сработает,т.к. "streams.first" не является названием локальной переменной
    // return streams.first;  // error: use of deleted function (вызов конструктора копирования для потока std::ifstream)

    // return std::move(streams).first;
    // return std::move(streams.first);  // нет разницы когда менять семантику - до обращения к полю или после.

    // можно сделать ещё проще
    return MakeStreams(prefix).first;
}

int main() {
    return 0;
}
