#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

// converts space-separated string "text" into vector of strings
vector<string> SplitIntoWords(const string& text) {
    istringstream iss(text);
    vector<string> words;
    string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    int relevance;
};

struct Query {
   public:
    set<string> minus_words;
    set<string> plus_words;

    bool IsQueryEmpty() const {
        if (plus_words.empty()) {
            return true;
        } else {
            return false;
        }
    }
};

class SearchServer {
   public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        ++_num_of_documents;
        const vector<string> words = SplitIntoWordsNoStop(document);

        for (const string word : words) {
            _inv_index[word].insert(document_id);
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_struct = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_struct);

        sort(matched_documents.begin(),
             matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;  // sort by descendin order
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

   private:
    map<string, set<int>> _inv_index;
    set<string> _stop_words;
    int _num_of_documents{0};

    bool IsStopWord(const string& word) const {
        return _stop_words.count(word) > 0;
    }

    vector<Document> FindAllDocuments(const Query& query_struct) const {
        // key:   id of found document
        // value: relevance of corresponding document
        map<int, int> id_relevance_mapping;
        vector<Document> id_relevance_vector;

        // process all plus-words
        for (const auto& plus_word : query_struct.plus_words) {
            if (_inv_index.find(plus_word) != _inv_index.end()) {
                for (const auto& doc_id : _inv_index.at(plus_word)) {
                    id_relevance_mapping[doc_id]++;
                }
            }
        }

        // process all minus-words, i.e. remove from id_relevance_mapping all the documents which contain given minus-word
        for (const auto& minus_word : query_struct.minus_words) {
            if (_inv_index.find(minus_word) != _inv_index.end()) {
                for (const auto& doc_id : _inv_index.at(minus_word)) {
                    id_relevance_mapping.erase(doc_id);
                }
            }
        }

        for (const auto& [doc_id, doc_relevance] : id_relevance_mapping) {
            id_relevance_vector.push_back({doc_id, doc_relevance});
        }

        return id_relevance_vector;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query_words;
        set<string> plusWordsExceptMinusWords;

        for (string& word : SplitIntoWordsNoStop(text)) {
            if (!word.empty()) {
                if (word[0] == '-') {
                    word = word.substr(1);
                    if (_stop_words.count(word) == 0) {  // word is NOT present in "stop_words_" set
                        query_words.minus_words.insert(word);
                    }
                } else {
                    query_words.plus_words.insert(word);
                }
            }
        }
        std::set_difference(query_words.plus_words.begin(), query_words.plus_words.end(),
                            query_words.minus_words.begin(), query_words.minus_words.end(),
                            std::inserter(plusWordsExceptMinusWords, plusWordsExceptMinusWords.end()));
        query_words.plus_words = plusWordsExceptMinusWords;

        return query_words;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments()) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}