#include <fstream>
#include <iostream>
// #include <istream>  // std::istream
#include <map>
#include <numeric>  // std::gcd
#include <set>
#include <sstream>
#include <stdexcept>  // std::domain_error
#include <string>
#include <vector>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
Задание по программированию
«Калькулятор обыкновенных дробей»

Итак, теперь у нас с вами есть полноценный тип для представления обыкновенных дробей:
класс Rational интегрирован в язык с помощью перегрузки операторов и не позволяет выполнять некорректные операции за счёт выбрасывания исключений.
Давайте на основе класса Rational создадим простой калькулятор для обыкновенных дробей.

Вам надо написать программу, которая считывает из стандартного ввода одну строку в формате:
ДРОБЬ_1 операция ДРОБЬ_2.
ДРОБЬ_1 и ДРОБЬ_2 имеют формат X/Y, где X — целое, а Y — целое неотрицательное число. операция — это один из символов '+', '-', '*', '/'.

Если ДРОБЬ_1 или ДРОБЬ_2 является некорректной обыкновенной дробью,
ваша программа должна вывести в стандартный вывод сообщение "Invalid argument".
Если считанная операция — это деление на ноль, выведите в стандартный вывод сообщение "Division by zero".
В противном случае выведите результат операции.
Пример

stdin               stdout
1/2 + 1/3           5/6
1/2 + 5/0           Invalid argument
4/5 / 0/8           Division by zero


Division by zero
*/

class Rational {
   public:
    Rational() = default;

    Rational(int new_numerator, int new_denominator) {
        if (new_denominator == 0) {
            throw std::invalid_argument("Invalid argument"s);
        }

        int gcd = std::gcd(new_numerator, new_denominator);
        _numerator = new_numerator / gcd;
        _denominator = new_denominator / gcd;

        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
    }

    int Numerator() const {
        return _numerator;
    }

    int Denominator() const {
        return _denominator;
    }

   private:
    int _numerator{0};
    int _denominator{1};
};

bool operator==(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() == rhs.Numerator()) &&
           (lhs.Denominator() == rhs.Denominator());
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    return {
        lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
        lhs.Denominator() * rhs.Denominator()};
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    return {lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator()};
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    if (rhs.Numerator() == 0) {
        throw std::domain_error("Division by zero"s);
    }

    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
}

std::istream& operator>>(std::istream& is, Rational& r) {
    int n{}, d{};
    char delim{};

    if (is >> n >> delim >> d && delim == '/') {
        r = Rational(n, d);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << r.Numerator() << '/' << r.Denominator();

    return os;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() * rhs.Denominator()) < (rhs.Numerator() * lhs.Denominator());
}

void PerformOperation(std::istream& input) {
#ifdef _GLIBCXX_DEBUG
    // === debug mode is enabled ===
    std::string path = "input.txt"s;
    std::ifstream in(path);

    // сохраняем указатель на "streambuf"
    std::streambuf* cinbuf_bkp = std::cin.rdbuf();
    // перенаправляем поток ввода std::cin на файл "input.txt"
    std::cin.rdbuf(in.rdbuf());  // redirect std::cin to "input.txt"!
#endif                           // END OF _GLIBCXX_DEBUG

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        Rational r1{}, r2{};
        char operation{};
        try {
            iss >> r1 >> std::ws >> operation >> std::ws >> r2 >> std::ws;

            switch (operation) {
                case '+':
                    std::cout << r1 + r2 << std::endl;
                    break;
                case '-':
                    std::cout << r1 - r2 << std::endl;
                    break;
                case '*':
                    std::cout << r1 * r2 << std::endl;
                    break;
                case '/':
                    std::cout << r1 / r2 << std::endl;
                    break;
                    // default:
                    //     std::cout << "Invalid operation"s << std::endl;
                    //     break;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << '\n';
        }
    }

#ifdef _GLIBCXX_DEBUG
    std::cin.rdbuf(cinbuf_bkp);  // восстанавливаем standard input
#endif
}

int main() {
    PerformOperation(std::cin);
    return 0;
}