#pragma once

#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>  // std::stable_partition
#include <iterator>   // std::distance
#include <utility>    // std::pair, std::move

#include "date.h"

class Database {
public:
    void Add(const Date &date, const std::string &event);

    void Print(std::ostream &os) const;

    template<typename Predicate>
    int RemoveIf(const Predicate &pred);

    template<typename Pred>
    std::multimap<Date, std::string> FindIf(Pred pred) const;

    std::string Last(const Date &date) const;

private:
    std::multimap<Date, std::string> _d2e;
    std::map<Date, std::set<std::string>> _checker;
};

// Метод возвращает количество удаленных записей, для которых предикат возвращает "true"
// и удаляет необходимые записи в базе данных
template<typename Predicate>
int Database::RemoveIf(const Predicate& pred) {
    std::multimap<Date, std::string> new_d2e;
    auto it = std::remove_copy_if(_d2e.begin(), _d2e.end(),
                                  std::inserter(new_d2e, new_d2e.begin()),
                                  [&pred](const auto &p) {
                                      return pred(p.first, p.second);
                                  });
    int counter = _d2e.size() - new_d2e.size();

    // актуализируем "_checker"
    _checker.clear();
    for (const auto& [date, event]: new_d2e) {
        _checker[date].insert(event);
    }

    _d2e = std::move(new_d2e);

    return counter;
}

template<typename Pred>
std::multimap<Date, std::string> Database::FindIf(Pred pred) const {
    std::multimap<Date, std::string> mm;
    for (const auto& [date, events] : _checker) {
        for (const auto& event : events) {
            if (pred(date, event)) {
                mm.template emplace(date, event);
            }
        }
    }
    return mm;
}

std::ostream& operator<<(std::ostream& os, const std::pair<const Date, std::string>& p);

