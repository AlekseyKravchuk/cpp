#include <iostream>
#include <string>
#include <vector>

/*
Задание по программированию «Перемещение строк»
Напишите функцию MoveStrings, которая принимает два вектора строк, source и destination,
и дописывает все строки из первого вектора в конец второго.
После выполнения функции вектор source должен оказаться пустым.
Чтобы очистить содержимое вектора, надо вызвать у него метод clear:

vector<string> words = {"hey", "hi"};
words.clear();
// Теперь вектор words пуст
 */

// TODO:
void MoveStrings(std::vector<std::string> source, std::vector<std::string> destination) {
    /*
    https://stackoverflow.com/questions/18159183/can-i-move-the-contents-of-one-vector-to-the-end-of-another

    You can use std::make_move_iterator, so that accesses to the iterator returns rvalue references instead of lvalue references:
    a.insert(a.end(), std::make_move_iterator(b.begin()), std::make_move_iterator(b.end()));

    ========================


    There is a std::move algorithm that appears to do what you want.
    In the following code the source std::vector is left with empty strings (the vector size doesn't change).
    int main()
    {
        std::vector<std::string> one{"cat", "dog", "newt"};
        std::vector<std::string> two;

        std::move(begin(one), end(one), back_inserter(two));

        std::cout << "one:\n";
        for (auto& str : one) {
            std::cout << str << '\n';
        }

        std::cout << "two:\n";
        for (auto& str : two) {
            std::cout << str << '\n';
        }
    }
     */
}

int main() {
    return 0;
}
