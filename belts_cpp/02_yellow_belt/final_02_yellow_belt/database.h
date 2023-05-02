#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "date.h"

using DB = std::map<Date, std::vector<std::string>>;

class Database {
   public:
    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& os) const;

    template <typename Pred>
    DB FindIf(Pred pred);

    template <typename Pred>
    int RemoveIf(Pred pred);

    std::string Last(const Date& date);

   private:
    DB _date2events;
};

template <typename Pred>
DB Database::FindIf(Pred pred) {
    // TODO: now it's just stub
    return DB{};
}

// метод возвращает количество удаленных записей, для которых предикат возвращает "true"
template <typename Pred>
int Database::RemoveIf(Pred pred) {
    // TODO: now it's just stub
    return 0;
}