#include <iostream>

using namespace std::literals;

class Animal {
   public:
    Animal(const std::string type) : _type(type) {}

    void Eat(const std::string fruit) {
        std::cout << _type << " eats "s << fruit << std::endl;
    }

    virtual void Voice() const {}

   private:
    const std::string _type;
};

class Cat : public Animal {
   public:
    Cat() : Animal("cat"s) {}

    void Voice() const override {
        std::cout << "Meow!" << std::endl;
    }
};

class Dog : public Animal {
   public:
    Dog() : Animal("dog"s) {}

    void Voice() const override {
        std::cout << "Whaf!" << std::endl;
    }
};

class Parrot : public Animal {
   public:
    Parrot(const std::string name) : Animal("parrot"s), _name(name) {}

    void Voice() const override {
        std::cout << _name << " is good!"s << std::endl;
    }

   private:
    const std::string _name;
};

void MakeSound(const Animal& animal) {
    animal.Voice();
}

void Test1() {
    Cat cat;
    Dog dog;
    Parrot parrot("Kesha"s);
    cat.Eat("apple"s);
    dog.Eat("orange"s);

    std::cout << "======================"s << std::endl;

    MakeSound(cat);
    MakeSound(dog);
    MakeSound(parrot);
}

int main() {
    Test1();
    return 0;
}