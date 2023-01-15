#include <iostream>
#include <string>

using namespace std::literals;

struct Fruit {
    int health = 0;
    std::string type = "fruit"s;
};

struct Apple : public Fruit {
    Apple() {
        health = 10;
        type = "apple";
    }
};

struct PineApple : public Fruit {
    PineApple() {
        health = 15;
        type = "pineapple";
    }
};

struct Orange : public Fruit {
    Orange() {
        health = 5;
        type = "orange";
    }
};

class Animal {
   public:
    std::string animalType{"animal"s};

    void Eat(const Fruit& f) {
        std::cout << animalType << " eats " << f.type << ". "s
                  << f.health << "hp." << std::endl;
    }
};

class Cat : public Animal {
   public:
    std::string animalType{"cat"s};

    void Meow() const {
        std::cout << "meow!"s << std::endl;
    }

    void Eat(const Fruit& f) {
        std::cout << animalType << " eats " << f.type << ". "s
                  << f.health << "hp." << std::endl;
    }
};

class Dog : public Animal {
   public:
   std::string animalType{"dog"s};

    void Eat(const Fruit& f) {
        std::cout << animalType << " eats " << f.type << ". "s
                  << f.health << "hp." << std::endl;
    }
};

int main() {
    Apple apple;
    Orange orange;
    PineApple pineapple;
    Cat cat;
    Dog dog;

    cat.Meow();
    cat.Eat(apple);
    cat.Eat(orange);
    cat.Eat(pineapple);

    dog.Eat(orange);
    dog.Eat(pineapple);

    return 0;
}
