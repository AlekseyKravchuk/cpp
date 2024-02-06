#include <algorithm> // std::min
#include <iostream>
#include <iterator> // std::next
#include <vector>

using namespace std;

template <typename T>
vector<T> Head(vector<T>& v, size_t top) {
    return {v.begin(), next(v.begin(), min(top, v.size()))};
}

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    // чтобы проитерироваться по вектору "v" у нас есть удобный механизм:
    for (const auto& elm : v) {
        cout << elm << ' ';
    }
    cout << endl;

    // а теперь мы хотим проитерироваться не по всему вектору, а по первым 3-м его элементам
    // мы уже не можем воспользоваться удобным циклом "range-based for loop"
    // придется писать обычный цикл со счетчиком (но это не так удобно, как в первом случае).
    for (size_t i = 0; i < 3; ++i) {
        cout << v[i] << ' ';
    }
    cout << endl;

    // поставим перед собой задачу написать следующую функцию "Head":
    for (int x : Head(v, 3)) {
        cout << x << ' ';
    }
    cout << endl;

    cout << "========================================" << endl;

    // Мы можем захотеть пройтись по префиксу нашего вектора и, например, увеличить каждый его элемент на 1
    for (int& x : Head(v, 3)) {
        ++x;
    }
    cout << endl;
    cout << "========================================" << endl;
    // ожидаем, что первые 3 элемента вектора увеличатся на 1, а 4-ый и 5-ый останутся без изменений
    for (int x : Head(v, 3)) {  // из этой записи логично ожидать, что мы меняем сам вектор
        cout << x << ' ';
    }
    // 1 2 3  // исходный вектор не изменился
    cout << endl;

    return 0;
}
