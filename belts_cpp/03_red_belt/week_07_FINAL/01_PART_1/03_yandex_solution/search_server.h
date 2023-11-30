#pragma once

#include "inverted_index.h"
#include "iterator_range.h"
#include "profile.h"

#include <array>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Good job! (Max time used: 39.35/54.50, max preprocess time used: 0/None, max memory used: 484302848/805306368.)

class SearchServer {
  private:
    InvertedIndex index;
    static const size_t MAX_SIZE = 50'001;

  public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

  private:
    std::vector<std::pair<int, int>> FormSearchResults(int, std::array<int, MAX_SIZE>&);

    std::string SingleThreadQuires(const std::string&);

    template <typename ResponseContainer>
    std::string CreateQueryResponse(string_view current_query,
                                    const ResponseContainer& resp) {
        std::ostringstream search_results_output;

        search_results_output << current_query << ':';
        for (auto [hitcount, docid] : Head(resp, 5)) {
            search_results_output << " {"
                                  << "docid: " << -docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << '\n';

        return search_results_output.str();
    }
};
