// разработайте сигнатуру фунции MakeVector по аналогии с функцией MakeSet из урока
/*
Напишите функцию MakeVector, аналогичную функции MakeSet из урока.
Она должна быть шаблоном, принимающим итераторы на начало и конец диапазона элементов
и возвращающим полученный из этих элементов вектор.
*/

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container>
void EraseAndPrint(Container& container, int posToDelete, int rangeBegin, int rangeEnd) {
    container.erase(container.begin() + posToDelete);
    PrintRange(container.begin(), container.end());

    container.erase(container.begin() + rangeBegin, container.begin() + rangeEnd);
    PrintRange(container.begin(), container.end());
}

template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 0, 0, 2);
    return 0;
} 
