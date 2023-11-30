#include "search_server.h"
#include "parse.h"
#include "profile.h"

#include <algorithm>
#include <unordered_map>

#include <functional>
#include <thread>
#include <future>

using Bucket = std::vector<std::pair<size_t, std::string>>;

auto CreateThreadBuckets(std::istream& document_input) {
  static const size_t MAX_THREADS = std::thread::hardware_concurrency();
  std::vector<Bucket> input_buckets(MAX_THREADS);  

  size_t document_index = 0;
  for (string current_document; getline(document_input, current_document); ) {
      input_buckets[document_index % MAX_THREADS].emplace_back(
              document_index,
              std::move(current_document)
      );
      ++document_index;
  }

  return std::pair{input_buckets, document_index};
}

InvertedIndex ProcessBucket(const Bucket& bucket) {
    InvertedIndex result;
    for (auto& [doc_id, doc] : bucket) {
        result.Add(doc_id, doc);
    }
    return result;
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    auto [thread_buckets, total_elements] = CreateThreadBuckets(document_input);

    std::vector<std::future<InvertedIndex>> futures;
    int ind = 0;
    for (auto &bucket: thread_buckets) {
        futures.push_back(std::async(ProcessBucket, std::cref(bucket)));
        ++ind;
    }

    InvertedIndex new_index;
    for (auto &fut: futures) {
        new_index.Insert(fut.get());
    }
    new_index.SwapHelperAndMainStorage();
    std::swap(index, new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  auto [thread_buckets, total_elements] = CreateThreadBuckets(query_input);

  std::vector<std::string> result(total_elements);
  std::vector<std::future<void>> futures;

  for (auto& bucket: thread_buckets) {
      futures.push_back(std::async([&bucket, this, &result] {
          for (auto &[input_id, input_document]: bucket) {
              result[input_id] = SingleThreadQuires(input_document);
          }
      }));
  }

  for (auto& fut : futures) {
      fut.get();
  }
  for (auto& str : result) {
      search_results_output << str;
  }
}

std::vector<std::pair<int, int>>
SearchServer::FormSearchResults(int non_zero_elements,
                                std::array<int, MAX_SIZE>& docid_count)
{
    std::vector<std::pair<int, int>> search_results;
    search_results.reserve(non_zero_elements);

    int doc_id = 0;
    for (auto count: docid_count) {
        if (count) {
            search_results.emplace_back(count, -doc_id);
        }
        ++doc_id;
    }
    std::partial_sort(search_results.begin(),
                      Advance(search_results.begin(), search_results.end(), 5),
                      search_results.end(), std::greater<>());

    return search_results;
}

std::string SearchServer::SingleThreadQuires(const std::string& current_query) {
    const auto words = SplitIntoWords(current_query);

    std::array<int, MAX_SIZE> docid_count{};
    int non_zero_elements = 0;
    for (const auto &[word, cnt]: words) {
        for (const auto &[doc_id, count]: index.Lookup(word)) {
            non_zero_elements += !docid_count[doc_id];
            docid_count[doc_id] += count * cnt;
        }
    }

    return CreateQueryResponse(
            current_query,
            FormSearchResults(non_zero_elements, docid_count)
    );
}
