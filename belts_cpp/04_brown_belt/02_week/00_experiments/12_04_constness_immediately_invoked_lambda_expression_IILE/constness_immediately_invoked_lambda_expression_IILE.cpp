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
    // const vector<int> sorted_numbers = Unique(Sorted({5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 8}));

    // Используем идиому IILE, чтобы не снимать константность с вектора "sorted_numbers"
    // и при этом выполнить некоторую его нетривиальную инициализацию рядом с его объявлением.
    // Суть IILE: инициализируем объект в лямбда-функции (возвращаем сформированный объект) и тут же вызываем лямбду.
    const vector<int> sorted_numbers = []() {
        vector<int> data = Sorted({5, 4, 2, 1, 5, 1, 3, 4, 5, 6, 5, 8, 1});

        auto it = unique(begin(data), end(data));
        data.erase(it, end(data));

        return data;
    }();

    // Идиома IILE бывает очень полезной, когда нам нужно замерить время конструирования объектаbelts_cpp/TESTING_FRAMEWORK/02_simple/profile.h

    cout << sorted_numbers << endl;

    return 0;
}
