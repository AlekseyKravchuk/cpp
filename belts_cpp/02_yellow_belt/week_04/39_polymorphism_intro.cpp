#include <iostream>

using namespace std::literals;

class Animal {
   public:
    Animal(const std::string type) : _type(type) {}

    void Eat(const std::string fruit) {
        std::cout << _type << " eats "s << fruit << std::endl;
    }

    void Voice() const {
        if (_type == "cat"s) {
            std::cout << "Meow!" << std::endl;
        } else if (_type == "dog"s) {
            std::cout << "Whaf!" << std::endl;
        } else if (_type == "parrot"s) {
            // в базовом классе неизвестно поле "name", которое является приватным в производном классе "Parrot"
            std::cout << _name << " is good!"s << std::endl;
        }
    }

   private:
    const std::string _type;
};

class Cat : public Animal {
   public:
    Cat() : Animal("cat"s) {}
};

class Dog : public Animal {
   public:
    Dog() : Animal("dog"s) {}
};

class Parrot : public Animal {
   public:
    Parrot(const std::string name) : Animal("parrot"s), _name(name) {}

   private:
    const std::string _name;
};

void MakeSound(const Animal& animal) {
    animal.Voice();
}

int main() {
    Cat cat;
    Dog dog;
    Parrot parrot("Kesha"s);
    cat.Eat("apple"s);
    dog.Eat("orange"s);

    std::cout << "======================"s << std::endl;

    MakeSound(cat);
    MakeSound(dog);
    MakeSound(parrot);

    return 0;
}