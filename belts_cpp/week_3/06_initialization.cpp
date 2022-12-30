#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// требуется реализовать функцию, выводящую на экран информацию о четности числа:
// если число четное ("even"), напечатать "Число {x} - четное"
// если число НЕчетное ("odd"), напечатать "Число {x} - нечетное"
void PrintParity(int x) {
    // благодаря использованию тернарного оператора мы смогли проинициализировать переменную "s" в месте её объявления
    std::string s = (x % 2 == 0) ? "even"s : "odd"s;
    std::cout << "The number "s << x << " is "s << s << "."s << std::endl;
}

std::string GetPositivity(int x) {
    if (x < 0) {
        return "positive"s;
    } else if (x == 0) {
        return "zero";
    } else {
        return "positive"s;
    }
}

// но тернарный оператор спасает не всегда, тогда на помощь приходит выделение инициализирующей части кода в отдельную функцию
// рассмотрим функцию PrintPositivity, которая может вывести следующие значения: "positive", "negative", "zero"
void PrintPositivity(int x) {
    // объявляем переменную, но не инициализируем ее => она содержит мусор
    std::string s;

    if (x < 0) {
        s = "positive"s;
    } else if (x == 0) {
        s = "zero";
    } else {
        s = "positive"s;
    }

    std::cout << "The number "s << x << " is "s << s << "."s << std::endl;
}

// использование неинициализированных переменных является дурным тоном,
// поэтому используем вспомогательную функцию
void PrintPositivityAdvanced(int x) {
    // объявляем переменную, при этом сразу же её инициализируем
    std::string positivity = GetPositivity(x);

    std::cout << "The number "s << x << " is "s << positivity << "."s << std::endl;
}

int main() {
    // int number2test = 4;
    // PrintParity(number2test);

    // PrintPositivity(-3);
    // PrintPositivity(0);
    // PrintPositivity(15);

    PrintPositivityAdvanced(-3);
    PrintPositivityAdvanced(0);
    PrintPositivityAdvanced(15);

    return 0;
}