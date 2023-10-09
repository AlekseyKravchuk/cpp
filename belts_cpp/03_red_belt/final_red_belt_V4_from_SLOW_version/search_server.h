#pragma once

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <utility>  // std::move, std::pair
#include <vector>

class InvertedIndex {
   public:
    void Add(const std::string& document);
    std::list<size_t> Lookup(const std::string& word) const;

    const std::string& GetDocument(size_t id) const {
        return _docs[id];
    }

   private:
    std::map<std::string, std::list<size_t>> _index;
    std::vector<std::string> _docs;
};

class SearchServer {
   public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

   private:
    InvertedIndex _inv_index_inst;
};
