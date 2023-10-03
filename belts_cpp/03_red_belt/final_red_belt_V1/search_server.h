#pragma once

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility> // std::move, std::pair
#include <vector>

// class InvertedIndex {
//   public:
//     void Add(const std::string& document);

//     std::list<size_t> Lookup(const std::string& word) const;

//     const std::string& GetDocument(size_t id) const {
//         return _docs[id];
//     }

//   private:
//     std::vector<std::string> _docs; // вектор поданных на вход строк
//     std::map<std::string_view, std::list<size_t>> _index;
// };

class SearchServer {
  public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

  private:
    // word_3 : {{doc_id_1 : 2},
    //           {doc_id_2 : 1},
    //            ...
    //           {doc_id_N : 5}}
    // 
    // word_1 : {{doc_id_4 : 2},
    //           {doc_id_6 : 1},
    //            ...
    //           {doc_id_N : 5}}

    //                   word                 doc_id  number_of_occurrences
    std::unordered_map<std::string, std::map<size_t, size_t>> _index;
};
