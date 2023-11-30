#pragma once

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>


// Good job! (Max time used: 50.78/54.50, max preprocess time used: 0/None, max memory used: 416092160/805306368.)

class InvertedIndex {
  public:
    void Add(std::string&& document);
    std::vector<std::pair<size_t, size_t>> Lookup(std::string&& word) const;
  private:
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> _index;  // {word1 : [{docid4 : 1}, {docid6 : 3}], ...}
    size_t _doc_count{0};
};

class SearchServer {
  public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

  private:
    InvertedIndex _inv_index;
};
