#include <iostream>
#include <string>

using namespace std::literals;

struct Fruit {
    int health = 0;
    std::string type = "fruit"s;
};

struct Apple : public Fruit {
    Apple() {
        Fruit::health = 10;
        Fruit::type = "apple";
    }
};

struct PineApple : public Fruit {
    PineApple() {
        Fruit::health = 15;
        Fruit::type = "pineapple";
    }
};

struct Orange : public Fruit {
    Orange() {
        Fruit::health = 5;
        Fruit::type = "orange";
    }
};

class Animal {
   protected:
    std::string animalType{"none_type_animal"s};

   public:
    Animal() = default;
    Animal(const std::string& s) : animalType(s) {}

    void Eat(const Fruit& fruit) const {
        std::cout << animalType << " eats " << fruit.type << ". "s
                  << fruit.health << "hp." << std::endl;
    }
};

class Cat : public Animal {
   public:
    // конструктор кошки
    // по открывающейся фигурной скобке "{" (в том коде, который генерирует компилятор)  осуществляется вызов конструктора базового класса
    Cat(const std::string catName = "cat"s) : Animal(catName) {
        // Следовательно, в тот момент, когда мы входим в тело конструктора, все переменные базового класса УЖЕ проинициализированы
        // animalType = "Cat"s;  // соответственно, ЗДЕСЬ просиходит ПРИСВАИВАНИЕ, а не инициализация
    }

    void Meow() const {
        std::cout << "meow!"s << std::endl;
    }
};

class Dog : /* protected */ public Animal {
   public:
    // конструктор собаки
    Dog() {
        animalType = "Dog"s;
    }
};

// создадим функцию DoMeal (покормить)
// в нее будем передавать некоторое животное и некоторый фрукт
// при этом функция не будет знать, какие конкретные животное и фрукт были ей переданы
// "DoMeal" принимает ссылки на объекты базового типа
void DoMeal(const Animal& animal, const Fruit& fruit) {
    // animal.animalType += "*"s;

    // благодаря тому, что спецификатор доступа для переменной класса "animalType",
    // определенной в классе "Animal", указан как "private",
    // доступ к ней ИЗВНЕ ОТСУТСТВУЕТ -
    // обращаться к ней могут только методы производных классов
    // std::cout << animal.animalType << std::endl;
    animal.Eat(fruit);
}

int main() {
    Apple apple;
    Orange orange;
    PineApple pineapple;
    Cat cat;
    Dog dog;

    Animal animal;
    animal.Eat(orange);

    Cat cat2("Murzik"s);
    cat2.Eat(apple);

    // метод Meow() определен внутри класса "Cat" => всё идёт хорошо
    // cat.Meow();

    // а здесь метод Eat() НЕ определен внутри класса "Cat",
    // НО, поскольку, наследование публичное от базового класса "Animal",
    // то в производном классе "Cat" мы имеем доступ к ПОЛЯМ и МЕТОДАМ базового класса
    // cat.Eat(orange);

    // аналогично
    // dog.Eat(pineapple);

    // std::cout << std::endl;
    // DoMeal(dog, orange);

    // cat.Eat(apple);
    // cat.Eat(orange);
    // cat.Eat(pineapple);

    // dog.Eat(orange);
    // dog.Eat(pineapple);

    return 0;
}
