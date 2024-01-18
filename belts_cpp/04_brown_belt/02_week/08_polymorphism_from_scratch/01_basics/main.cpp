#include <iostream>
#include <memory>   // std::shared_ptr
#include <utility>  // std::move
#include <vector>

using namespace std;

// ================= Investigating inner workings of virtual functions mechanism =================
// g++ -Wall -g -std=c++17 file_name.cpp -fdump-lang-class -o output_file_name

// В результате использования флага "-fdump-lang-class" будет создан файл "*.cpp.001l.class"
// Далее в этом файле поиском ищем "Vtable for A", где "А" - это название исследуемого класса.
// В возможные значения строки поиска: "Vtable for Animal", "Vtable for Cat", Vtable for Dog" и т.д.
// ================================================================================================

// класс "Animal" - абстрактный, т.к. имеет чисто виртуальную функцию
// мы не можем ооздавать объекты (инстанцировать) объекты абстрактных классов
class Animal {
   public:
    Animal(const string& type) : _type(type) {}

    void Eat(const string& fruit) {
        cout << _type << " eats " << fruit << endl;
    }

    // Требуем от всех классов-потомков !!!ОБЯЗАТЕЛЬНОЙ!!! реализации метода "Voice()".
    // Теперь виртуальный метод "Voice()" является абстрактным (или "чисто виртуальным").
    virtual void Voice() const = 0;  // pure virtual method

   private:
    const string _type;
};

class Cat : public Animal {
   public:
    Cat() : Animal("cat") {}

    virtual void Voice() const override {
        cout << "Meow!" << endl;
    }
};

class Dog : public Animal {
   public:
    Dog() : Animal("dog") {}

    void Voice() const override {
        cout << "Whaf!" << endl;
    }
};

class Parrot : public Animal {
   public:
    Parrot(const string& name)
        : Animal("parrot"),
          _name(name) {}

    void Voice() const override {
        cout << "Tju-tju-tju!" << endl;
    }

    void Talk() {
        cout << _name << " is good!" << endl;
    }

   private:
    const string& _name;
};

class Horse : public Animal {
   public:
    Horse()
        : Animal("horse") {}

    void Voice() const override {
        cout << "Pffffrrrr (horse)" << endl;
    }
};

void MakeSound(const Animal& animal) {
    animal.Voice();
}

int main() {
    // shared_ptr<Animal> a;
    // a = make_shared<Cat>();
    // a = make_shared<Dog>();
    // a = make_shared<Parrot>("Zyablik");
    // a->Voice();

    Cat c;
    Dog d;

    vector<shared_ptr<Animal>> animals = {
        make_shared<Cat>(),
        make_shared<Dog>(),
        make_shared<Parrot>("Kesha"),
        make_shared<Cat>(move(c)),
        make_shared<Horse>(),
        make_shared<Dog>(d)};

    for (const auto& animal_ptr : animals) {
        // animal_ptr->Voice();
        MakeSound(*animal_ptr);
    }

    cout << "=========" << endl;

    c.Voice();

    return 0;
}
