#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>  // std::prev
#include <list>
#include <map>
#include <utility>  // std::move

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#endif  //_GLIBCXX_DEBUG

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    std::ios::sync_with_stdio(false);

    // а также выполняется отвязка std::cin от std::cout
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    REDIRECT_INPUT("input.txt"s);
#endif  //_GLIBCXX_DEBUG

    const int MAX_ATHLETES = 100'000;
    using Position_Iterator = std::list<int>::iterator;

    int n_athletes;
    std::cin >> n_athletes;

    std::list<int> lst;
    std::vector<Position_Iterator> athlete_pos(MAX_ATHLETES + 1, lst.end());

    for (int i = 0; i < n_athletes; ++i) {
        int athlete, next_athlete;
        std::cin >> athlete >> next_athlete;
        athlete_pos[athlete] = lst.insert(athlete_pos[next_athlete],
                                          athlete);
    }

    for (int x : lst) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    return 0;
}