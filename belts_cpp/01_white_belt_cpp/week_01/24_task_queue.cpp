#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

/* 
Люди стоят в очереди, но никогда не уходят из её начала, зато могут приходить в конец и уходить оттуда.
Более того, иногда некоторые люди могут прекращать и начинать беспокоиться из-за того, что очередь не продвигается.
Реализуйте обработку следующих операций над очередью:

    WORRY i: пометить i-го человека с начала очереди (в нумерации с 0) как беспокоящегося;
    QUIET i: пометить i-го человека как успокоившегося;
    COME k: добавить k спокойных человек в конец очереди;
    COME -k: убрать k человек из конца очереди;
    WORRY_COUNT: узнать количество беспокоящихся людей в очереди.
Изначально очередь пуста.

Формат ввода:
    Количество операций Q, затем описания операций.
    Для каждой операции WORRY i и QUIET i гарантируется, что человек с номером i существует в очереди на момент операции.
    Для каждой операции COME -k гарантируется, что k не больше текущего размера очереди.

Формат вывода:
Для каждой операции WORRY_COUNT выведите одно целое число — количество беспокоящихся людей в очереди.
 
Пример:

Ввод:
8
COME 5
WORRY 1
WORRY 4
COME -2
WORRY_COUNT
COME 3
WORRY 3
WORRY_COUNT

Вывод:
1
2
 */

enum class OPERATIONS { COME,
                        WORRY,
                        WORRY_COUNT };

std::map<std::string, OPERATIONS> str2op = {
    {"COME", OPERATIONS::COME},
    {"WORRY", OPERATIONS::WORRY},
    {"WORRY_COUNT", OPERATIONS::WORRY_COUNT}};

void ProcessOperations(int n) {
    std::vector<bool> v;

    for (int i = 0; i < n; ++i) {
        std::string str;
        std::cin >> str;

        switch (str2op[str]) {
            case OPERATIONS::COME:
                int k;
                std::cin >> k;
                v.resize(v.size() + k);
                break;
            case OPERATIONS::WORRY:
                int i;
                std::cin >> i;
                v[i] = true;
                break;
            case OPERATIONS::WORRY_COUNT:
                std::cout << std::count_if(v.begin(), v.end(), [](bool value) {
                    return value;
                }) << std::endl;
                break;

            default:
                break;
        }
    }
}

int main() {
    int n{};
    std::cin >> n;
    ProcessOperations(n);

    return 0;
}
