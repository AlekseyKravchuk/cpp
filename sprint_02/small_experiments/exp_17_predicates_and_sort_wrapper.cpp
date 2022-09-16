#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

void PrintNames(const vector<Animal>& animals) {
    for (const Animal& animal : animals) {
        cout << animal.name << ' ';
    }
    cout << endl;
}

template <typename Container, typename FieldExtractor>
void SortBy(Container& container, FieldExtractor extr, bool reverse = false) {
    if (reverse) {  // sort by DESCENDING order
        auto comparator = [extr](const auto& lhs, const auto& rhs) { return extr(lhs) > extr(rhs); };
        sort(container.begin(), container.end(), comparator);
    } else {  // sort by ASCENDING order
        auto comparator = [extr](const auto& lhs, const auto& rhs) { return extr(lhs) < extr(rhs); };
        sort(container.begin(), container.end(), comparator);
    }
}

int main() {
    vector<Animal> animals = {
        {"Мурка"s, 10, 5},
        {"Белка"s, 5, 1.5},
        {"Георгий"s, 2, 4.5},
        {"Рюрик"s, 12, 3.1},
        {"Евгений", 25, 8}};

    // без сортировки: Мурка Белка Георгий Рюрик
    PrintNames(animals);

    // сортировка по имени: Белка Георгий Мурка Рюрик
    SortBy(animals, [](const Animal& animal) { return animal.name; });
    PrintNames(animals);

    // сортировка по убыванию веса: Мурка Георгий Рюрик Белка
    SortBy(
        animals, [](const Animal& animal) { return animal.weight; }, true);
    PrintNames(animals);
}