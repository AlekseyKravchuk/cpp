#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#include "log_duration.h"

using namespace std;

bool TestPermutUsingSorting(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    vector<int> v1_sorted = v1;
    std::sort(v1_sorted.begin(), v1_sorted.end());

    vector<int> v2_sorted = v2;
    std::sort(v2_sorted.begin(), v2_sorted.end());

    return v1_sorted == v2_sorted;
}

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    // если дошли сюда, значит векторы одинакового размера
    int len = v1.size();
    std::map<int, int> counter1, counter2;

    for (int i = 0; i < len; ++i) {
        ++counter1[v1[i]];
        ++counter2[v2[i]];
    }

    return counter1 == counter2;
}

bool TestPermutNaive(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    for (int i : v1) {
        // проверяем, что каждый элемент первого вектора
        // содержится одинаковое количество раз в обоих векторах
        if (count(v1.begin(), v1.end(), i) != count(v2.begin(), v2.end(), i)) {
            return false;
        }
    }

    return true;
}

int main() {
    std::mt19937 g;

    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    // оба вектора случайны, вряд ли они совпадут
    cout << "Random vectors match? "s << flush;
    {
        LOG_DURATION("TestPermut"s);
        cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
    }

    cout << "Random vectors match? "s << flush;
    {
        LOG_DURATION("TestPermutNaive"s);
        cout << (TestPermutNaive(v1, v2) ? "Yes"s : "No"s) << endl;
    }

    cout << "Random vectors match? "s << flush;
    {
        LOG_DURATION("TestPermutUsingSorting"s);
        cout << (TestPermutUsingSorting(v1, v2) ? "Yes"s : "No"s) << endl;
    }

    // делаем один перестановкой другого явным образом
    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);
    cout << "Permuted vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}