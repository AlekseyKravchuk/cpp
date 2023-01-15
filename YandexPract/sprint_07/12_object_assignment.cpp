#include <cassert>
#include <string>

using namespace std;

struct Cat {
    string name;
    int age = 0;

    Cat& operator=(const Cat& rhs) {
        // Обратите внимание на защиту от присваивания объекта самому себе.
        // Это идиоматичный способ избежать лишних действий и некорректной работы при самоприсваивании объекта.
        if (this != &rhs) {
            name = rhs.name;
            age = rhs.age;
        }

        // this - указатель на текущий экземпляр класса.
        // В операции присваивания он ссылается на левый аргумент операции.
        return *this;  // возвращаем ссылку на левый аргумент операции присваивания
    }
};

int main() {
    Cat cat1{"Tom"s, 3};
    Cat cat2;

    // Сгенерированный компилятором метод operator= выполнит
    // присваивание соответствующих полей класса Cat.
    cat2 = cat1;

    assert(cat1.name == cat2.name);
    assert(cat1.age == cat2.age);
}