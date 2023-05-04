#pragma once

#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>

#include "date.h"

using Entries = std::map<Date, std::vector<std::string>>;
using num_deleted_entries = uint64_t;

class Database {
   public:
    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& os) const;

    template <typename Pred>
    num_deleted_entries RemoveIf(Pred pred);
    
    template <typename Pred>
    Entries FindIf(Pred pred);

    std::string Last(const Date& date);

   private:
    uint64_t _counter{};
    std::map<Date, std::map<std::string, uint64_t>> _date_to_events;
    std::map<Date, std::map<uint64_t, std::string>> _date_to_ins_order;
};

// Метод возвращает количество удаленных записей, для которых предикат возвращает "true"
// и удаляет необходимые записи в базе данных
template <typename Pred>
num_deleted_entries Database::RemoveIf(Pred pred) {
    // TODO: now it's just stub
    return 0;
}

template <typename Pred>
Entries Database::FindIf(Pred pred) {
    // TODO: now it's just stub
    return Entries{};
}
