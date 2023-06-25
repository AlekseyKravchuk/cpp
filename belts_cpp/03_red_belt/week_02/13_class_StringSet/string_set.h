#pragma once

#include <iterator>  // std::prev
#include <set>
#include <string>
#include <deque>

class StringSet {
   public:
    void Add(const std::string& s, int priority) {
        _data.push_back(s);
        _sorted_data.insert(StringItem{_data.back(), priority});
    }

    const std::string& FindLast() const {
        return _data.back();
    }

    const std::string& FindBest() const {
        return std::prev(_sorted_data.end())->s;
    }

   private:
    struct StringItem {
        std::string& s;
        int priority;
        bool operator<(const StringItem& other) const {
            return priority < other.priority;
        }
    };

    std::deque<std::string> _data;
    std::set<StringItem> _sorted_data;
};