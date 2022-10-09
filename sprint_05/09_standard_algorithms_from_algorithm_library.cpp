#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    // удобный тип ostringstream -> https://ru.cppreference.com/w/cpp/io/basic_ostringstream
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

template <class ForwardIterator>
vector<string> GetPermutations(ForwardIterator first, ForwardIterator last) {
    vector<string> resultPermutaions;

    sort(first, last);
    do {
        resultPermutaions.push_back(PrintRangeToString(first, last));
    } while (next_permutation(first, last));

    return resultPermutaions;
}

int main() {
    vector<int> permutation(3);
    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    int startValue = 1;
    iota(permutation.begin(), permutation.end(), startValue);
    auto result = GetPermutations(permutation.begin(), permutation.end());
    for (const auto& s : result) {
        cout << s;
    }
    return 0;
}
