#include <iostream>
#include <string>
#include <utility>  // std::as_const

using namespace std::literals;

class Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const {
        std::cout << "\?\?\?!"s << std::endl;
    }
};

class Cat : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Meow!"s << std::endl;
    }
};

class Dog : public Animal {
   public:
    // перегруженные виртуальные методы должны иметь ОДИНАКОВУЮ сигнатуру
    virtual void Voice() const override {
        std::cout << "Whaf!"s << std::endl;
    }
};

void MakeSound(const Animal* p) {
    p->Voice();
}

int main() {
    Animal a;
    Cat c;
    Dog d;

    MakeSound(&a);
    MakeSound(&c);
    MakeSound(&d);

    return 0;
}
