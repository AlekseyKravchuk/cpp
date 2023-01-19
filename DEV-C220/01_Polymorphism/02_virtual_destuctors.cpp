#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const {
        std::cout << "\?\?\?!"s << std::endl;
    }

    ~Animal() {
        std::cout << "Destructor Animal."s << std::endl;
    }
};

class Cat : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Meow!"s << std::endl;
    }

    ~Cat() {
        std::cout << "Destructor Cat."s << std::endl;
    }
};

class Dog : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Whaf!"s << std::endl;
    }

    ~Dog() {
        std::cout << "Destructor Dog."s << std::endl;
    }
};

void MakeSound(Animal& animal) {
    animal.Voice();
}

int main() {
    std::vector<Animal*> zoo{new Animal(), new Cat(), new Dog(), new Cat()};
    zoo[0]->Voice();

    // error: deleting object of polymorphic class type ‘Animal’
    // which has non-virtual destructor might cause undefined behavior [-Werror=delete-non-virtual-dtor]
    // delete zoo[1];

    zoo[1]->Voice();
    zoo[2]->Voice();

    return 0;
}
