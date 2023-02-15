#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std::literals;

/* 
Задание по программированию «Средняя температура»
Даны значения температуры, наблюдавшиеся в течение N подряд идущих дней. Найдите номера дней (в нумерации с нуля) со значением температуры выше среднего арифметического за все N дней.
Гарантируется, что среднее арифметическое значений температуры является целым числом.
 
Формат ввода:
    Вводится число N, затем N неотрицательных целых чисел — значения температуры в 0-й, 1-й, ... (N−1)-й день.

Формат вывода
    Первое число K — количество дней, значение температуры в которых выше среднего арифметического. Затем Kцелых чисел — номера этих дней.

Пример

Ввод:
5
7 6 3 0 9

Вывод:
3
0 1 4
 */

int main() {
    int N{};
    std::cin >> N;
    std::vector<int> v;
    v.reserve(N);

    int value{};
    while (std::cin >> value) {
        v.push_back(value);
    }

    int avg = std::accumulate(v.begin(), v.end(), 0) / v.size();

    std::vector<int> indices;
    for (auto it = v.begin(); it != v.end(); ++it) {
        if(*it > avg) {
            indices.push_back(it - v.begin());
        }
    }

    std::cout << indices.size() << std::endl;
    bool isFirst = true;
    for (const auto& elm : indices) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << ' ' << elm;
        }
    }

    return 0;
}
