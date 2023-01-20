#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std::literals;

class Animal {
   public:
    Animal(const std::string& type) : _type(type) {}

    void Eat(const std::string& fruit) const {
        std::cout << _type << " eats " << fruit << std::endl;
    }

    // с помощью ключевого слова "virtual" мы говорим базовому классу о том, что
    // в классах-потомках (в производных классах) может быть реализация метода "Voice"
    // с точно такой же сигнатурой, т.е. возвращаемый тип, константность, типы и количество параметров должны совпадать
    // virtual void Voice() const {
    //     std::cout << _type << " is silent."s << std::endl;
    // }

    // делаем виртуальный метод Voice виртуальным (pure virtual, абстрактный)
    // тем самым в каждом классе-потомке мы ТРЕБУЕМ его обязательной реализации
    virtual void Voice() const = 0;

    // virtual void Voice() const {
    //     std::cout << _type << " is silent"s << std::endl;
    // }

   private:
    const std::string _type{};
};

class Cat : public Animal {
   public:
    Cat() : Animal("cat"s) {}

    void Voice() const override {
        std::cout << "Meow!"s << std::endl;
    }
};

class Dog : public Animal {
   public:
    Dog() : Animal("dog"s) {}

    void Voice() const override {
        std::cout << "Whaf!"s << std::endl;
    }
};

class Horse : public Animal {
   public:
    Horse() : Animal("horse"s) {}

    void Voice() const override {
        std::cout << "Phrrrrrrrrrrrrr!"s << std::endl;
    }
};

class Parrot : public Animal {
   public:
    Parrot(const std::string name) : Animal("parrot"s), _name(name) {}

    void Voice() const {
        std::cout << _name << " is good!" << std::endl;
    }

   private:
    const std::string _name;
};

void MakeSound(const Animal& animal) {
    animal.Voice();
}

// // в MakeSound передаем указатель на Animal
// void MakeSound(const Animal* const animal) {
//     animal->Voice();
// }

int main() {
    std::vector<std::shared_ptr<Animal>> animals = {
        std::make_shared<Dog>(),
        std::make_shared<Cat>(),
        std::make_shared<Horse>(),
        std::make_shared<Parrot>("Kesha"s)
    };

    for (auto animal : animals) {
        MakeSound(*animal);
    }

    return 0;
}
