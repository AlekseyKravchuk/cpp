#include <iostream>
#include <vector>
#include <memory>  // std::shared_ptr, std::make_shared

using namespace std::literals;

// ================= Investigating inner workings of virtual functions mechanism =================
// g++ -Wall -g -std=c++17 file_name.cpp -fdump-lang-class -o output_file_name

// В результате использования флага "-fdump-lang-class" "*.cpp.001l.class"
// ================================================================================================

class Animal {
   public:
    Animal(const std::string type) : _type(type) {}

    void Eat(const std::string fruit) {
        std::cout << _type << " eats "s << fruit << std::endl;
    }

    // требуем, чтобы этот метод был реализован во всех классах-потомках
    // делаем метод Sound чисто виртуальным (pure virtual) или абстрактным
    // virtual void Sound() const = 0;

    // virtual void Sound() const {
    //     std::cout << _type << " is silent"s << std::endl;
    // }

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

// класс Horse отличается от предыдущих тем, что в нем не определен метод Sound
class Horse : public Animal {
   public:
    Horse() : Animal("horse"s) {}
};

// Если мы передаем объекты по значению, а не по ссылке, то вызывается метод Sound из базового класса,
// а не из нужного нам производного класса.
// Это происходит т.к. при передаче объекта по значению мы теряем всю информацию об объекте производного класса (класса-потомка)
// и у нас сохраняется информация только об объекте базового класса
void MakeSound(const Animal& animal) {
    animal.Sound();
}

void Test1() {
    // Cat cat;
    // Dog dog;
    // Parrot parrot("Kesha"s);

    // // в классе Horse у нас нет реализации абстрактного метода Voice (задан в базовом абстрактном классе Animal),
    // // поэтому мы не можем создавать экземпляры такого класса
    // Horse horse;

    // cat.Eat("apple"s);
    // dog.Eat("orange"s);
    // parrot.Eat("banana"s);
    // std::cout << "======================"s << std::endl;

    // хотим объединить объекты разных классов (но в одной иерархии наследования) в контейнере
    std::vector<std::shared_ptr<Animal>> animals = {
        // в качестве шаблонного параметра в std::make_shared нужно указывать РЕАЛЬНЫЙ тип объекта, который мы хотим создать
        std::make_shared<Cat>(),
        std::make_shared<Dog>(),

        // в конструктор класса Parrot обязательно нужно передать строку с именем попугая;
        // параметры std::make_shared<T> будут переданы в конструктор объекта "T"
        std::make_shared<Parrot>("Vasya"s)
    };
    
    for (const auto animal_shptr : animals) {
        MakeSound(*animal_shptr);
        // animal_shptr->Sound();
    }
}

int main() {
    Test1();
    return 0;
}