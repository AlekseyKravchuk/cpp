#include <iostream>
#include <string>

using namespace std::literals;

struct Log {
    Log(const std::string name) : _name(name) {
        std::cout << "+ " << name << std::endl;
    }

    ~Log() {
        std::cout << "- " << _name << std::endl;
    }

    std::string _name;
};

struct Fruit {
    int _health = 0;
    std::string _type = "fruit"s;
    const Log log;

    Fruit(const std::string& s) : _type(s), log(s + " Fruit"s) {}
};

struct Apple : public Fruit {
    Apple(const std::string& s = "apple"s) : Fruit(s), log(s + " Apple"s) {
        Fruit::_health = 10;
        Fruit::_type = "apple";
    }

    const Log log;
};

int main() {
    Apple apple;

    return 0;
}
