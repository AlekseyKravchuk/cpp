#pragma once

#include <deque>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class InvertedIndex {
   public:
    void Add(const std::string&& document);
    std::vector<std::pair<size_t, size_t>> Lookup(std::string_view) const;

    const std::string& GetDocument(size_t id) const {
        return docs[id];
    }

   private:
    std::map<std::string_view, std::vector<std::pair<size_t, size_t>>> _index;
    std::deque<std::string> _docs;
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
