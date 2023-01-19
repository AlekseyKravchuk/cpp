#include <iostream>
#include <string>

using namespace std::literals;

struct Log {
    Log(std::string name) : _name(name) {
        std::cout << "+ " << name << std::endl;
    }

    ~Log() {
        std::cout << "- " << _name << std::endl;
    }

    std::string _name;
};

struct Fruit {
    int health = 0;
    std::string type = "fruit"s;
    Log log{"Fruit"s};
};

struct Apple : public Fruit {
    Apple() {
        Fruit::health = 10;
        Fruit::type = "apple";
        Log log{"Apple"s};
    }
};

int main() {
    Apple apple;

    return 0;
}
