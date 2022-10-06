#include <algorithm>
#include <iostream>
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

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    // Присвоим локальной переменной значение итератора начала вектора
    auto it_begin = langs.begin();
    auto it_end = langs.end();
    auto it = find(it_begin, it_end, "C++"s);
    // Здесь контейнер изменяется, а вместе с ним и все его итераторы!
    langs.insert(it, "C"s);
    // Обновим значения переменных
    it_begin = langs.begin();
    it_end = langs.end();
    // Теперь все точно сработает правильно
    PrintRange(it_begin, it_end);
}