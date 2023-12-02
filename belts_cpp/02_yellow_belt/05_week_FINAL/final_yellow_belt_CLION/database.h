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

using DateEventPair = std::pair<Date, std::string>;

class Database {
public:
    void Add(const Date &date, const std::string &event);

    void Print(std::ostream &os) const;

    template<typename Predicate>
    int RemoveIf(const Predicate &pred);

    template<typename Pred>
    std::vector<DateEventPair> FindIf(const Pred &pred) const;

    std::string Last(const Date &date) const;

private:
    std::map<Date, std::vector<std::string>> _d2e;
    std::map<Date, std::set<std::string>> _checker;
};

// Возвращает кол-во удаленных записей, для которых pred возвращает "true" и удаляет необходимые за2017-07-08писи в базе данных
template<typename Predicate>
int Database::RemoveIf(const Predicate &pred) {
    int counter_events_to_delete{};
    for (auto it = _d2e.begin(); it != _d2e.end(); ) {
        const auto& date = (*it).first;
        auto& events = (*it).second;
        auto it_first_to_save = std::stable_partition(events.begin(), events.end(),
                                                      [&](const std::string& event) {
                                                         auto tmp = pred(date, event);
                                                         return tmp;
                                                     });

        if (it_first_to_save != events.begin()) {
            counter_events_to_delete += std::distance(events.begin(), it_first_to_save);

            // актуализируем "_checker"
            for (auto it_to_del = events.begin(); it_to_del != it_first_to_save; ++it_to_del) {
                _checker[date].erase(*it_to_del);
            }

            // удяляем все события, для которых предикат "pred" вернул "true"
            events.erase(std::begin(events), it_first_to_save);
        }

        // если в результате удаления событий по предикату, возникли даты с пустыми векторами событий, удаляем их
        it = events.empty() ? _d2e.erase(it) : std::next(it);
    }

    return counter_events_to_delete;
}

template<typename Pred>
std::vector<DateEventPair> Database::FindIf(const Pred &pred) const {
    const int mult = 10;
    std::vector<DateEventPair> entries;
    entries.reserve(static_cast<int>(_d2e.size()) * mult);
    for (const auto& [date, events]: _d2e) {

        for (const auto& event: events) {
            if (pred(date, event)) {
                entries.emplace_back(date, event);
            }
        }
    }
    return entries;
}

std::ostream &operator<<(std::ostream &os, const std::pair<Date, std::string> &p);

