#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

const string& GetName(const Animal& animal) {
    return animal.name;
}

template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper) {
    // теперь можно сортировать контейнер
    // с компаратором key_mapper(lhs) < key_mapper(rhs)
} 

int main() {
    vector<Animal> animals = {
        {"Мурка"s, 10, 5},
        {"Белка"s, 5, 1.5},
        {"Георгий"s, 2, 4.5},
        {"Рюрик"s, 12, 3.1},
    };
    // auto key_mapper = GetName;
    auto key_mapper = [](const Animal& animal) { return animal.name; };
    cout << key_mapper(animals.front()) << endl;
    // выведет имя первого животного: Мурка
}