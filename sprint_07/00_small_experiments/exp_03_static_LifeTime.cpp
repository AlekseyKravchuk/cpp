#include <iostream>

using namespace std;

int value = 0;

void Fn() {
    // Глобальная переменная value существует в единственном экземпляре.
    // Адрес переменной value будет тот же, какой был получен в функции main
    cout << "Fn(): &value="s << &value << endl;
    value = 1;
}

int main() {
    cout << "main(): &value="s << &value << endl;
    cout << "value="s << value << endl;
    Fn();
    // Функция Fn изменила значение переменной value. Это изменение будет
    // видно и в функции main
    cout << "value="s << value << endl;
}