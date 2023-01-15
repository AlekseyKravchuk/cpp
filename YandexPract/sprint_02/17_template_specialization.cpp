#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

enum class AnimalSortKey {
    AGE,             // по полю age
    WEIGHT,          // по полю weight
    RELATIVE_WEIGHT  // по weight / age
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

void SortBy(vector<Animal>& animals, AnimalSortKey sort_key, bool reverse = false) {
    switch (sort_key) {
        case AnimalSortKey::AGE:
            return SortBy(animals, [](const auto& x) { return x.age; }, reverse);
        case AnimalSortKey::WEIGHT:
            return SortBy(animals, [](const auto& x) { return x.weight; }, reverse);
        case AnimalSortKey::RELATIVE_WEIGHT:
            return SortBy(animals, [](const auto& x) { return x.weight / x.age; }, reverse);
    }
}


// Этот вариант функции не компилируется: вначале осуществляется попытка проверить тип "KeyMapper" у функции
/* template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper, bool reverse = false) {
    // если KeyMapper — это AnimalSortKey...
    // if (is_same_v<KeyMapper, AnimalSortKey>) {
    if constexpr (is_same_v<KeyMapper, AnimalSortKey>) {
        switch (key_mapper) {
            case AnimalSortKey::AGE:
                return SortBy(container, [](const auto& x) { return x.age; }, reverse);
            case AnimalSortKey::WEIGHT:
                return SortBy(container, [](const auto& x) { return x.weight; }, reverse);
            case AnimalSortKey::RELATIVE_WEIGHT:
                return SortBy(container, [](const auto& x) { return x.weight / x.age; }, reverse);
        }
    }

    if (reverse) {
        sort(container.begin(), container.end(),
             [key_mapper](const auto& lhs, const auto& rhs) {
                 return key_mapper(lhs) > key_mapper(rhs);
             });
    } else {
        sort(container.begin(), container.end(),
             [key_mapper](const auto& lhs, const auto& rhs) {
                 return key_mapper(lhs) < key_mapper(rhs);
             });
    }
} */

int main() {
    vector<Animal> animals = {
        {"Мурка"s, 10, 5},
        {"Белка"s, 5, 1.5},
        {"Георгий"s, 2, 4.5},
        {"Рюрик"s, 12, 3.1},
    };

    // Мурка Белка Георгий Рюрик
    PrintNames(animals);

    // Рюрик Мурка Георгий Белка
    SortBy(
        animals, [](const Animal& animal) { return animal.name; }, true);
    PrintNames(animals);

    // ожидаем вывод: Рюрик Белка Мурка Георгий
    SortBy(animals, AnimalSortKey::RELATIVE_WEIGHT);
    PrintNames(animals);
}
