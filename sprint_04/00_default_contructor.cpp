#include <iostream>

using namespace std;

class Rational {
   public:
    // Явно сообщаем компилятору, что в этом классе нужно создать конструктор по умолчанию
    Rational() = default;

    // Параметризованный конструктор
    // Если в классе объявлен параметризованный конструктор, конструктор по умолчанию для этого класса сгенерирован не будет.
    // Наличие конструктора с параметрами как бы сообщает компилятору: "Этот класс требует особой инициализации".
    // Если конструктор по умолчанию всё-таки нужен, вы можете написать его сами либо попросить компилятор.
    // Для этого вместо тела конструктора укажите = default и задайте полям значения по умолчанию.
    Rational(int numerator, int denominator) {
        _numerator = numerator;
        _denominator = denominator;
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    // Задаем значения по умолчанию для числителя и знаменателя
    int _numerator = 0;
    int _denominator = 1;
};

int main() {
    const Rational scale(1, 3);

    cout << scale.Numerator() << "/"s << scale.Denominator() << endl;
}
