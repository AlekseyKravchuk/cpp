#pragma once

#include <cstddef> // size_t
#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include <vector>

class SearchServer {
  public:
    SearchServer() = default;

    explicit SearchServer(std::istream& document_input);

    void UpdateDocumentBase(std::istream& document_input);

    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

  private:
    std::unordered_map<std::string_view, std::map<size_t, size_t>> _index;
    std::unordered_set<std::string> _unique_words;
};
