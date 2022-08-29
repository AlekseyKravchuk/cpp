#include <algorithm>
#include <iostream>
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

/* // Yandex version of SplitIntoWords
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
} */

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
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        _documents.push_back({document_id, words});
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_struct = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_struct);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

   private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    vector<DocumentContent> _documents;
    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
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
                    if (stop_words_.count(word) == 0) {  // word is NOT present in "stop_words_" set
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

    vector<Document> FindAllDocuments(const Query& query_struct) const {
        vector<Document> matched_documents;

        for (const auto& document : _documents) {
            const int relevance = MatchDocument(document, query_struct);
            if (relevance > 0) {
                matched_documents.push_back({document.id, relevance});
            }
        }
        return matched_documents;
    }

    static int MatchDocument(const DocumentContent& content,
                             const Query& query_struct) {
        if (query_struct.IsQueryEmpty()) {
            return 0;
        }

        set<string> matched_words;

        for (const string& word : content.words) {
            // if query contains "word" that exists in document then the document is excluded from search result by default
            if (query_struct.minus_words.count(word)) {
                return 0;
            }
            // if there is at leat one match have been already occured for the given word
            if (matched_words.count(word) != 0) {
                continue;
            }

            if (query_struct.plus_words.count(word) != 0) {
                matched_words.insert(word);
            }
        }
        return static_cast<int>(matched_words.size());
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
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}