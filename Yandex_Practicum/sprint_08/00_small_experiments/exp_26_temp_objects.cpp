#include <iostream>
#include <string>

using namespace std;

class Number {
   private:
    int a_;

   public:
    Number() {
        cout << "Default Ctor"s << endl;
    }

    Number(int num) : a_(num) {
        cout << "Ctor(int)"s << endl;
    }

    Number(const Number& other) : a_(other.a_) {
        cout << "Copy Ctor"s << endl;
    }

    ~Number() {
        cout << "Dtor"s << endl;
    }

    Number& operator=(const Number& other) {
        a_ = other.a_;
        cout << "Op="s << endl;
        return *this;
    }

    Number operator+(const Number& second) {
        cout << "Op+" << endl;

        // В этом месте создается временный объект
        // У этого объекта ("a_ + second.a_") нет имени. Невозможно узнать его адрес. 
        // Но он должен быть создан, чтобы функция могла его вернуть.
        return a_ + second.a_;

    }
};

int main() {
    // Number a(1), b(1), c;

    Number a(1);
    Number b(1);
    Number c;

    // После вызыва operator=, временный объект будет скопирован в объект "c" и сразу после этого будет уничтожен.
    c = a + b;
    cout << "End of calculations"s << endl;
}