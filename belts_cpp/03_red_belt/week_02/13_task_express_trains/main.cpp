#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

/*
================= Задание по программированию «Экспрессы» =================
Вам даны задача и её решение — верное, но не удовлетворяющее заданным ограничениям на время работы. Переработайте это решение и сдайте в систему.
Условие:
Разработайте простейшую систему маршрутизации экспрессов, курсирующих по одному железнодорожному направлению,
представляющему собой прямую. Ваша программа должна уметь обрабатывать запросы двух типов:
    - ADD start finish — добавить в систему маршрутов экспресс, следующий со станции start до станции finish и обратно.
      Экспресс не делает промежуточных остановок. Станции задаются целыми числами, равными их расстоянию от вокзала (он имеет номер 0).
    - GO start finish — попытаться проложить беспересадочный маршрут от станции start до станции finish.
      Если существует экспресс между этими двумя станциями, в ответ на данный запрос выведите 0.
      В противном случае выведите положительное число — минимальное расстояние, на которое можно приблизиться к станции finish, стартовав строго на станции start и использовав не более одного экспресса.

Формат входных данных:
В первой строке вводится количество запросов Q — натуральное число, не превосходящее 10^5.
В следующих Qстроках в соответствии с описанным выше форматом вводятся запросы.
Гарантируется, что номера станций являются целыми числами, по модулю не превосходящими 10^9.

Формат выходных данных:
Для каждого запроса GO выведите единственное целое неотрицательное число — минимальное расстояние до конечной станции маршрута,
вычисляемое в соответствии с описанными выше правилами.

Ограничения:
1 секунда на выполнение всех запросов.
Все описанные в условии гарантии действительно справедливы для всех тестов, на которых будет запускаться ваша программа.
Проверять корректность тестов не нужно.

Пример
Ввод:
7
ADD -2 5
ADD 10 4
ADD 5 8
GO 4 10
GO 4 -2
GO 5 0
GO 5 100

Вывод:
0
6
2
92
*/

class RouteManagerSlow {
   public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].push_back(finish);
        reachable_lists_[finish].push_back(start);
    }
    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        if (reachable_lists_.count(start) < 1) {
            return result;
        }
        const std::vector<int>& reachable_stations = reachable_lists_.at(start);
        if (!reachable_stations.empty()) {
            result = std::min(result,
                              abs(finish - *std::min_element(
                                               std::begin(reachable_stations),
                                               std::end(reachable_stations),
                                               [finish](int lhs, int rhs) {
                                                   return abs(lhs - finish) < abs(rhs - finish);
                                               })));
        }
        return result;
    }

   private:
    std::map<int, std::vector<int>> reachable_lists_;
};

class RouteManager {
   public:
    void AddRoute(int start, int finish) {
        _reachables[start].insert(finish);
        _reachables[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);

        if (!_reachables.count(start)) {
            return result;
        }

        if (_reachables.at(start).count(finish)) {
            return 0;
        }

        const std::set<int>& reachable_stations = _reachables.at(start);
        if (!reachable_stations.empty()) {
            std::set<int>::const_iterator it = reachable_stations.upper_bound(finish);

            if (it == reachable_stations.begin()) {
                result = std::min(result, abs(finish - *it));
            } else if (it == reachable_stations.end()) {
                result = std::min(result, abs(finish - *std::prev(it)));
            } else {
                result = std::min(result, std::min(std::abs(finish - *std::prev(it)), std::abs(finish - *it)));
            }
        }
        return result;
    }

   private:
    std::map<int, std::set<int>> _reachables;
};

void RunSlowStub() {
    RouteManagerSlow routes;
    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        int start, finish;
        std::cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            std::cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }
}

void RunSolution() {
    RouteManager routes;
    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        int start, finish;
        std::cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            std::cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution"s);
            RunSolution();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("SlowStub"s);
            RunSlowStub();
        }
    }

#else
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}
