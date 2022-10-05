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

template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}

int main() {
    set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    vector<string> langs = MakeVector(unique_langs.begin(), unique_langs.end());
    PrintRange(langs.begin(), langs.end());

    return 0;
}
