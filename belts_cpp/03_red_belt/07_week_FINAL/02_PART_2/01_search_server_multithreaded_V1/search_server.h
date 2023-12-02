#pragma once

#include "search_server.h"
#include "synchronized.h"

#include <future>
#include <istream>
#include <map>
#include <ostream>
#include <queue>
#include <string>
#include <string_view>
#include <vector>


class InvertedIndex {
  public:
    struct Entry {
        size_t docid, hitcount;
    };

    InvertedIndex() = default;
    explicit InvertedIndex(std::istream& document_input);

    const std::vector<Entry>& Lookup(std::string_view word) const;

    const std::deque<std::string>& GetDocuments() const {
        return _docs;
    }

  private:
    std::deque<std::string> _docs;
    std::map<std::string_view, std::vector<Entry>> _index;
};

class SearchServer {
  public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input)
        : _inv_index(InvertedIndex(document_input)) {
    }

    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

  private:
    Synchronized<InvertedIndex> _inv_index;
    std::vector<std::future<void>> async_tasks;
};
