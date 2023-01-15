#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

// ========== Structures used by program ==========
struct Document {
    int id;
    double relevance;
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
// ==================================================

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

/* // My version of SplitIntoWords()
// converts space-separated string "text" into vector of strings
vector<string> SplitIntoWords(const string& text) {
    istringstream iss(text);
    vector<string> words;
    string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
} */

// Yandex version of SplitIntoWords()
vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

class SearchServer {
   public:
    void SetStopWords(const string& text) {
        if (!text.empty()) {
            for (const string& word : SplitIntoWords(text)) {
                _stop_words.insert(word);
            }
        }
    }

    void setQueryPlusWordsIDFs(const set<string>& plus_words) const {
        for (const string& plus_word : plus_words) {
            // calculate IDF (Inverse Document Frequency) for the given plus-word from query
            if (_inv_index.count(plus_word) != 0) {
                // _document_count: the total number of documents in Data Base
                // _inv_index_TF.at(plus_word).size(): the number of documents where "plus_word" occurs
                _word_IDF[plus_word] = log(_document_count / static_cast<double>(_inv_index.at(plus_word).size()));
            }
        }
    }

    void AddDocument(int doc_id, const string& document) {
        if (!document.empty()) {
            const vector<string> wordsNoStop = SplitIntoWordsNoStop(document);
            _document_count++;

            // populate inverted index with data: {"word_N":  { {doc_id_K: TF_of_word_N}, ... }
            // _inv_index_TF[word][doc_id] gets access to TF of given word within document (excluding STOP-words)
            for (const string word : wordsNoStop) {
                _inv_index[word][doc_id] += 1.0 / wordsNoStop.size();
            }
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_struct = ParseQuery(raw_query);
        map<int, double> matched_documents = FindAllDocuments(query_struct);
        vector<Document> topDocs;

        if (matched_documents.empty()) {
            return topDocs;
        }

        for (auto& [docID, relevance] : matched_documents) {
            topDocs.push_back({docID, relevance});
        }

        sort(topDocs.begin(),
             topDocs.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (lhs.relevance != rhs.relevance) {
                     return lhs.relevance > rhs.relevance;  // sort by relevance in descending order
                 } else {
                     return lhs.id > rhs.id;  // sort by docID if document relevances are equal
                 }
             });

        if (topDocs.size() > MAX_RESULT_DOCUMENT_COUNT) {
            topDocs.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return topDocs;
    }

   private:
    map<string, map<int, double>> _inv_index;
    set<string> _stop_words;
    int _document_count{0};
    mutable map<string, double> _word_IDF;

    bool IsStopWord(const string& word) const {
        return _stop_words.count(word) > 0;
    }

    map<int, double> FindAllDocuments(const Query& query_struct) const {
        if (query_struct.IsQueryEmpty()) {
            map<int, double> empty_map;
            return empty_map;
        }

        // get IDF for all plus-words from query
        setQueryPlusWordsIDFs(query_struct.plus_words);

        map<int, double> id_relevance;  // key: document ID; value: relevance of the document

        // calculate relevance of each document
        for (const string& plus_word : query_struct.plus_words) {
            if (_inv_index.count(plus_word) != 0) {
                for (const auto& [docID, wordTF] : _inv_index.at(plus_word)) {
                    id_relevance[docID] += _word_IDF.at(plus_word) * _inv_index.at(plus_word).at(docID);
                }
            }
        }

        // process all minus-words, i.e. remove from "id_relevance" map all the documents containing given minus-word
        if (!query_struct.minus_words.empty()) {
            for (const auto& minus_word : query_struct.minus_words) {
                if (_inv_index.count(minus_word) != 0) {
                    for (const auto& [docID, _] : _inv_index.at(minus_word)) {
                        id_relevance.erase(docID);
                    }
                }
            }
        }

        return id_relevance;
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

    Query ParseQuery(const string& raw_query) const {
        Query query_words;
        set<string> plusWordsExceptMinusWords;

        for (string& word : SplitIntoWordsNoStop(raw_query)) {
            if (!word.empty()) {
                if (word[0] == '-') {
                    word = word.substr(1);
                    if (_stop_words.count(word) == 0) {  // word is NOT present in "_stop_words" set
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
    const string raw_query = ReadLine();

    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(raw_query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }

    return 0;
}