#pragma once

#include <deque>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------
class InvertedIndex {
   public:
    void Add(std::string&& document);
    std::vector<std::pair<size_t, size_t>> Lookup(std::string_view word) const;

    const std::string& GetDocument(size_t id) const {
        return docs[id];
    }

   private:
    std::map<std::string_view, std::vector<std::pair<size_t, size_t>>> index;
    std::deque<std::string> docs;
};
//----------------------------------------------------------------------------------------------------
class SearchServer {
   public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

   private:
    InvertedIndex index;
};
//----------------------------------------------------------------------------------------------------