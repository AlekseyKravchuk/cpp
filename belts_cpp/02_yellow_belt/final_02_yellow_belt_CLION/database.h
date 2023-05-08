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
#include <stdexcept>
#include <vector>

#include "date.h"

using Entries = std::vector<std::pair<Date, std::vector<std::string>>>;

class Database {
public:
    void Add(const Date &date, const std::string &event);

    void Print(std::ostream &os) const;

    template<typename Predicate>
    int RemoveIf(const Predicate &pred);

    template<typename Pred>
    Entries FindIf(const Pred& pred) const;

    std::string Last(const Date &date) const;

private:
    std::map<Date, std::vector<std::string>> _d2e;
    std::map<Date, std::set<std::string>> _checker;
};

// Возвращает кол-во удаленных записей, для которых pred возвращает "true" и удаляет необходимые записи в базе данных
template<typename Predicate>
int Database::RemoveIf(const Predicate &pred) {
    int counter{};
    for (auto &[date, events]: _d2e) {
        auto it = std::stable_partition(events.begin(), events.end(),
                                        [&](const std::string &event) {
                                            return pred(date, event);
                                        });
        counter += std::distance(events.begin(), it);

        // актуализируем "_checker"
        for (const auto it_to_del = events.begin(); it_to_del != it; ++it) {
            _checker[date].erase(*it_to_del);
        }

        // удяляем все события, для которых предикат "pred" вернул "true"
        events.erase(std::begin(events), it);
    }

    return counter;
}

template<typename Pred>
Entries Database::FindIf(const Pred& pred) const {
    Entries entries;

    for (const auto& p : _d2e) {
//        auto it = std::stable_partition(p.second.begin(), p.second.end(),
//                                        [](const std::string& event) {
//                                            return true;
//                                        });
//        entries.push_back({date, std::vector<std::string>(events.begin(), it)});
    }
    return entries;
}

std::ostream& operator<<(std::ostream &os, const std::pair<Date, std::vector<std::string>>& p);

