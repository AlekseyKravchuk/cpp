#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

// Good job! (Max time used: 45.61/54.50, max preprocess time used: 0/None, max memory used: 416100352/805306368.)

class InvertedIndex {
  public:
    void Add(std::string&& document);
    std::vector<std::pair<size_t, size_t>> Lookup(std::string_view word) const;

  private:
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> _index; // {word : [{docid1, count2}, ..., {docidN, countN}]}
    size_t _doccount{0};
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
