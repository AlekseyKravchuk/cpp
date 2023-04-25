#include <iostream>

using namespace std::literals;

/*
Предположим, мы хотим написать некий волшебный мир, в котором есть некие животные, например,
кошки и собаки. Они могут есть различные фрукты: яблоки, апельсины и т.д. А у фруктов есть
некоторое количество здоровья.
*/

struct Log {
    Log(std::string loger_name) : _loger_name(loger_name) {
        std::cout << "+ " << _loger_name << std::endl;
    }

    ~Log() {
        std::cout << "- " << _loger_name << std::endl;
    }

    // идентификатор логера
    std::string _loger_name;
};

class Fruit {
   public:
    Fruit(const int health, const std::string& fruit_type)
        : _health(health),
          _fruit_type(fruit_type),
          _log(_fruit_type + " (Fruit Base Class)"s) {}

    int GetHealth() const { return _health; }

    std::string GetFruitType() const { return _fruit_type; }

   protected:
    const int _health;
    const std::string _fruit_type;
    const Log _log;
};

// яблоко является наследником фрукта
class Apple : public Fruit {
   public:
    Apple(const std::string& id) : Fruit(10, "apple "s + id), _log("apple "s + id + " (Derived Part)"s) {}
    const Log _log;
};

class AppleTree {
   public:
    AppleTree() : _a1("a1"s), _a2("a2"s), _log("AppleTree"s) {}

   private:
    const Apple _a1;
    const Apple _a2;
    const Log _log;
};

// апельсин тоже является наследником фрукта
class Orange : public Fruit {
   public:
    Orange() : Fruit(5, "orange"s) {}
};

// вводим ещё одного наследника
class Pineapple : public Fruit {
   public:
    Pineapple() : Fruit(20, "pineapple"s) {}
};

class Animal {
   public:
    Animal(const std::string t = "default_animal"s)
        : _animal_type(t) {}

    virtual void Voice() const = 0;
    virtual void Eat(const Fruit& fruit) const = 0;

   protected:
    std::string _animal_type = ""s;
};

class Cat : public Animal {
   public:
    Cat() {
        Animal::_animal_type = "Cat"s;
    }

    void Voice() const override {
        std::cout << "meow!"s << std::endl;
    }

    void Eat(const Fruit& fruit) const override {
        std::cout << _animal_type << " eats "s << fruit.GetFruitType() << ". Health = "s << fruit.GetHealth() << "."s << std::endl;
    }
};

class Dog : public Animal {
   public:
    Dog() : Animal("Dog"s) {
    }

    void Voice() const override {
        std::cout << "gav!"s << std::endl;
    }

    void Eat(const Fruit& fruit) const override {
        std::cout << _animal_type << " eats "s << fruit.GetFruitType() << ". Health = "s << fruit.GetHealth() << "."s << std::endl;
    }
};

void DoMeal(const Animal& animal, const Fruit& fruit) {
    animal.Eat(fruit);
}

void TestInheritance() {
    Cat cat;
    cat.Voice();
    Apple apple("a1"s);
    cat.Eat(apple);
    Orange orange;
    cat.Eat(orange);

    // хотим накормить кошку ещё и ананасом
    Pineapple pineapple;
    cat.Eat(pineapple);

    // ещё раз покормим кошечку апельсином, только на этот раз с помощью функции "DoMeal"
    DoMeal(cat, orange);

    std::cout << "===============================" << std::endl;
    // добавляем собаку и демонстрируем все её возможности
    Dog dog;
    dog.Voice();
    dog.Eat(apple);
    dog.Eat(orange);

    // хотим накормить собаку ананасом
    dog.Eat(pineapple);
}

void TestConstructionDestruction() {
    Apple apple1("a1"s);
    Apple apple2("a2"s);
}

void TestAppleTree() {
    // Apple apple1("a1"s);
    // Apple apple2("a2"s);
    AppleTree();
}

int main() {
    // TestInheritance();
    // TestConstructionDestruction();
    TestAppleTree();
    return 0;
}