#include <iostream>

/*
Задание по программированию «Максимизатор»
Напишите функцию UpdateIfGreater, которая принимает два целочисленных аргумента: first и second.
Если first оказался больше second, Ваша функция должна записывать в second значение параметра first.
При этом указанная функция не должна ничего возвращать, а изменение параметра second должно быть видно на вызывающей стороне.
 */

void UpdateIfGreater(const int& first, int& second) {
    if (first > second) {
        second = first;
    }
}

int main() {
    int a = 4;
    int b = 2;
    UpdateIfGreater(a, b);

    return 0;
}
