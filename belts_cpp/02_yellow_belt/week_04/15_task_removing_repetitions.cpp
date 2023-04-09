#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

/*
Удаление повторов
Напишите шаблонную функцию RemoveDuplicates, принимающую по ссылке вектор elements объектов типа T и удаляющую из него все дубликаты элементов.
Порядок оставшихся элементов может быть любым.
Гарантируется, что объекты типа T можно сравнивать с помощью операторов ==, !=, < и >.

template <typename T>
void RemoveDuplicates(vector<T>& elements);
*/

// // моя изначальная реализация
// template <typename T>
// void RemoveDuplicates(vector<T>& elements) {
//     set<T> s(elements.begin(), elements.end());
//     elements.clear();
//     for (const auto& elm : s) {
//         elements.push_back(elm);
//     }
// }

template <typename T>
void RemoveDuplicates(vector<T>& elements) {
    std::sort(elements.begin(), elements.end());
    elements.erase(std::unique(elements.begin(), elements.end()), elements.end());
}

int main() {
    vector<int> v1 = {6, 4, 7, 6, 4, 4, 0, 1};
    RemoveDuplicates(v1);
    for (int x : v1) {
        cout << x << " ";
    }
    cout << endl;

    vector<string> v2 = {"C", "C++", "C++", "C", "C++"};
    RemoveDuplicates(v2);
    for (const string& s : v2) {
        cout << s << " ";
    }
    cout << endl;
    return 0;
}