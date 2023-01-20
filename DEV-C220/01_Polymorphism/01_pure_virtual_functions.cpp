#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

// как только в классе появляется чисто виртуальный метод, сам класс становится АБСТРАКТНЫМ
// => компилятор не даст создавать объекты такого класса
class Animal {
   public:
    virtual void Voice() const = 0;

    virtual ~Animal() {
        std::cout << "Destructor Animal."s << std::endl;
    }
};

class Cat : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Meow!"s << std::endl;
    }

    virtual ~Cat() override {
        std::cout << "Destructor Cat."s << std::endl;
    }
};

class Dog : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Whaf!"s << std::endl;
    }

    virtual ~Dog() override {
        std::cout << "Destructor Dog."s << std::endl;
    }
};

void MakeSound(Animal& animal) {
    animal.Voice();
}

int main() {
    Cat cat;
    Animal* animalPtr = &cat;
    animalPtr->Voice();

    return 0;
}
