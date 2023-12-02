#include <iostream>

using namespace std::literals;

class Animal {
   public:
    Animal(const std::string type) : _type(type) {}

    void Eat(const std::string fruit) {
        std::cout << _type << " eats "s << fruit << std::endl;
    }

    // требуем, чтобы этот метод был реализован во всех классах-потомках
    // делаем метод Sound чисто виртуальным (pure virtual) или абстрактным
    virtual void Sound() const = 0;

   private:
    const std::string _type;
};

class Cat : public Animal {
   public:
    Cat() : Animal("cat"s) {}

    void Sound() const override {
        std::cout << "Meow!" << std::endl;
    }
};

class Dog : public Animal {
   public:
    Dog() : Animal("dog"s) {}

    void Sound() const override {
        std::cout << "Whaf!" << std::endl;
    }
};

class Parrot : public Animal {
   public:
    Parrot(const std::string name) : Animal("parrot"s), _name(name) {}

    void Sound() const override {
        std::cout << _name << " is good!"s << std::endl;
    }

   private:
    const std::string _name;
};

// у класса Horse нет реализации АБСТРАКТНОГО метода "Sound" => класс Horse является АБСТРАКТНЫМ
// Базовый класс Animal также является АБСТРАКТНЫМ
class Horse : public Animal {
   public:
    Horse() : Animal("horse"s) {}
};

void MakeSound(const Animal& animal) {
    animal.Sound();
}

void Test1() {
    Cat cat;
    Dog dog;
    Parrot parrot("Kesha"s);

    // // в классе Horse у нас нет реализации абстрактного метода Voice (задан в базовом абстрактном классе Animal),
    // // поэтому мы не можем создавать экземпляры такого класса
    // Horse horse;

    cat.Eat("apple"s);
    dog.Eat("orange"s);
    parrot.Eat("banana"s);

    std::cout << "======================"s << std::endl;

    MakeSound(cat);
    MakeSound(dog);
    MakeSound(parrot);
}

int main() {
    Test1();
    return 0;
}