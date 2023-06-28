#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>  // std::prev
#include <list>
#include <map>
#include <utility>  // std::move

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#endif  //_GLIBCXX_DEBUG

class SportsmenOrdering {
   public:
    void PutInOrder(int athlete, int next_athlete) {
        if (_athlete_to_iter.count(next_athlete)) {
            auto iter = _lst.insert(_athlete_to_iter[next_athlete], athlete);
            _athlete_to_iter.insert({athlete, iter});
        } else {
            _lst.push_back(athlete);
            _athlete_to_iter.insert({athlete, std::prev(_lst.end())});
        }
    }

    void PrintInOrder() {
        bool isFirst = true;
        for (const auto& number : _lst) {
            if (!isFirst) {
                std::cout << ' ' << number;
            } else {
                isFirst = false;
                std::cout << number;
            }
        }
        std::cout << std::endl;
    }

   private:
    std::list<int> _lst{};
    std::map<int, std::list<int>::const_iterator> _athlete_to_iter{};
};

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    std::ios::sync_with_stdio(false);

    // а также выполняется отвязка std::cin от std::cout
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    REDIRECT_INPUT("input.txt"s);
#endif  //_GLIBCXX_DEBUG

    int n{};  // n <= 100'000
    std::cin >> n;
    SportsmenOrdering ordering;
    int number{}, prev_number{};

    for (int i = 0; i < n; ++i) {
        std::cin >> number >> prev_number;
        ordering.PutInOrder(number, prev_number);
    }

    ordering.PrintInOrder();

    return 0;
}