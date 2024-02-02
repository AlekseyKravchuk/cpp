#include <algorithm>
#include <iostream>
#include <vector>

#include "print.h"
#include "profile.h"

using namespace std;

vector<int> Sorted(vector<int> data) {
    sort(begin(data), end(data));
    return data;
}

vector<int> Unique(vector<int> data) {
    auto it = unique(begin(data), end(data));
    data.erase(it, end(data));
    return data;
}

int main() {
    // // const vector<int> sorted_numbers = Unique(Sorted({5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 8}));

    // // Используем идиому IILE, чтобы не снимать константность с вектора "sorted_numbers"
    // // и при этом выполнить некоторую его нетривиальную инициализацию рядом с его объявлением.
    // // Суть IILE: инициализируем объект в лямбда-функции (возвращаем сформированный объект) и тут же вызываем лямбду.
    // const vector<int> sorted_numbers = []() {
    //     vector<int> data = Sorted({5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 5, 8, 1});

    //     auto it = unique(begin(data), end(data));
    //     data.erase(it, end(data));

    //     return data;
    // }();

    // Идиома IILE бывает очень полезной, когда нам нужно замерить время конструирования объекта

    // при использовании IILE крайне не рекомендуется использовать спецификатор "auto"
    // в типе инициализируемой с помощью IILE переменной
    const vector<int> sorted_numbers = []() {
        // если бы мы поместили LOG_DURATION в функцию Sorted, то тогда бы замерялись все вызовы этой функции,
        // а нам нужно замерить только этот конкретный вызов функции Sorted.
        LOG_DURATION("Sorted numbers build");
        return Sorted({5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 5, 8, 1});
    }();

    cout << sorted_numbers << endl;

    return 0;
}
