#pragma once

#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "parse.h"

class InvertedIndex {
    using HelperCounter   = std::unordered_map<int, int>;
    using HelperContainer = std::unordered_map< std::string_view, HelperCounter>;
    using MainCounter     = std::vector<std::pair<int, int>>;
    using MainContainer   = std::unordered_map< std::string_view, MainCounter >;

  public:
    void Add(size_t, const string&);
    void InsertCount(string_view word, size_t doc_id, size_t count);
    void Insert(const InvertedIndex& other);
    void SwapHelperAndMainStorage();
    const MainCounter& Lookup(string_view word) const;

  private:
    HelperContainer helper_storage;
    MainContainer main_storage;
    inline static const MainCounter EMPTY;
    std::list<std::string> data;
};
