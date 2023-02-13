#include <algorithm>  // std::copy
#include <iostream>
#include <iterator>  // std::make_move_iterator
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

// 1-ая версия: "в лоб"
// void MoveStrings(std::vector<std::string>& source, std::vector<std::string>& destination) {
//     for (const auto& str : source) {
//         destination.push_back(str);
//     }
//     source.clear();
//     source.shrink_to_fit();
// }

// 2-ая версия: с использованием move-семантики
void MoveStrings(std::vector<std::string>& source, std::vector<std::string>& destination) {
    destination.reserve(source.size() + destination.size());
    // std::copy(std::make_move_iterator(source.begin()),
    //           std::make_move_iterator(source.end()),
    //           std::back_inserter(destination));

    std::move(source.begin(),
              source.end(),
              std::inserter(destination, destination.end()));

    source.clear();
    source.shrink_to_fit();
}

int main() {
    std::vector<std::string> source = {"a", "b", "c"};
    std::vector<std::string> destination = {"z"};
    MoveStrings(source, destination);

    return 0;
}
