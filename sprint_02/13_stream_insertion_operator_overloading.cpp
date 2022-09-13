#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// TO DO!!!!!!!!!!!!!!!
// задание 6/6
template <typename ContainerType>
void Print(ostream& out, const ContainerType& container) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        if (next(it) != container.end()) {
            out << *it << ", "s;
        } else {
            out << *it;
        }
    }
}

template <typename elemType>
ostream& operator<<(ostream& out, const vector<elemType>& container) {
    out << "["s;
    Print(out, container);
    out << "]"s;

    return out;
}

template <typename elemType>
ostream& operator<<(ostream& out, const set<elemType>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;

    return out;
}

template <typename KeyType, typename ValueType>
ostream& operator<<(ostream& out, const map<KeyType, ValueType>& dict) {
    out << "<"s;
    Print(out, dict);
    out << ">"s;

    return out;
}

// Yandex version
/* template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", "s;
        }
        is_first = false;
        out << element;
    }
    return out;
} */

int main() {
    // const vector<int> ages = {10, 5, 2, 12, 17};
    // cout << ages << endl;

    // const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    // cout << cats << endl;

    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    const map<string, int> cat_ages = {
        {"Мурка"s, 10},
        {"Белка"s, 5},
        {"Георгий"s, 2},
        {"Рюрик"s, 12}};
    cout << cat_ages << endl;

    return 0;
}