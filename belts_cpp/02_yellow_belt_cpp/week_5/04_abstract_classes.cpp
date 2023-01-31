#include <iostream>
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

// перегружаем MakeSound для того, чтобы обеспечить работу с указателями
void MakeSound(const Animal* animalPtr) {
    animalPtr->Voice();
}

int main() {
    Cat c;
    Dog d;
    Parrot p("Kesha"s);
    Horse h;

    // MakeSound(c);
    // MakeSound(d);
    // MakeSound(p);
    // MakeSound(h);
    // std::vector<Animal&> v{&Cat{}, &Dog{}, &Parrot{"Kesha"s}, &Horse{}};
    
    // Создать вектор из ссылок на нужный нам класс НЕВОЗМОЖНО, поскольку
    // The component type of containers like vectors must be assignable.
    // References are not assignable (you can only initialize them once when they are declared,
    // and you cannot make them reference something else later).
    // Other non-assignable types are also not allowed as components of containers, e.g. vector<const int> is not allowed.
    // std::vector<Animal&> v{c, d, p, h};  // ERROR !!!

    // с указателями работать можно
    std::vector<Animal*> animals{&c, &d, &p, &h};
    for (auto animalPtr : animals) {
        MakeSound(animalPtr);
    }

    // Cat c;
    // Dog d;
    // Parrot p("Kesha"s);
    // Horse h;

    // MakeSound(c);
    // MakeSound(d);
    // MakeSound(p);
    // MakeSound(h);

    return 0;
}
