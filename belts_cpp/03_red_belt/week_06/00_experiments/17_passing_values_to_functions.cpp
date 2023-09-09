#include <iostream>
#include <string>
#include <utility> // std::move

class Data {
  public:
    Data() = default;

    Data(const Data&) {
        std::cout << "call Data copy constructor" << std::endl;
    }

    Data(Data&&) {
        std::cout << "call Data MOVE constructor" << std::endl;
    }
};

// В функциях и методах, в которых предполагается, что передаваемые в них данные, будут отобраны у источника данных,
// !!! РЕКОМЕНДУЕТСЯ ВСЕГДА передавать по значению, а потом перемещать!!!
class Test1 {
  public:
    // Если в конструктор "Test1" передадим "lvalue", то у нас сначала произойдет копирование, т.к. принимаем по значению,
    // а потом перемещение: 1) Copy-Constructor -> 2) Move-Constructor
    explicit Test1(Data value) // <== передаем "value" по значению
        : _value(std::move(value)) {
        std::cout << "-----" << std::endl;
    }

  private:
    Data _value;
};

// в классе "Test2" присутствует лишний метод (конструктор)
class Test2 {
  public:
    explicit Test2(const Data& value)
        : _value(value) {
        std::cout << "-----" << std::endl;
    }

    explicit Test2(Data&& value)
        : _value(std::move(value)) {
        std::cout << "-----" << std::endl;
    }

  private:
    Data _value;
};

int main() {
    {
        // копирование->перемещение: Copy -> Move
        Data data;
        Test1 test(data);
    }

    {
        // перемещение->перемещение: Move -> Move
        Data data;
        Test1 test(std::move(data));
    }

    {
        //
        std::cout << "NRVO?" << std::endl;
        Test1 test(Data());
        std::cout << "-----" << std::endl;
    }

    {
        // ОДНО копирование: ОДИН вызов конструктора копирования: Copy
        Data data;
        Test2 test(data); // вызывается конструктор копирования, который принимает константную ссылку
    }

    {
        // ОДНО перемещение: ОДИН вызов конструктора перемещения: Move
        Data data;
        Test2 test(std::move(data));
    }
}