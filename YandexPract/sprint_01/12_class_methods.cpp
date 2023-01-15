#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>  // for istringstream
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    int id;
    int relevance;
};

/* bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
} */

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

set<string> ParseStopWords(const string& text) {
    set<string> stop_words;

    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

class SearchServer {
   private:
    struct DocumentContent {
        int id = 0;            // document ID
        vector<string> words;  // vector of words composing document
    };

    /*     struct Document {
        int id;
        int relevance;
    }; */

   private:
    vector<DocumentContent> _documents;
    set<string> _stop_words;

    set<string> ParseQuery(const string& text) const {
        set<string> query_words;

        for (const string& word : SplitIntoWordsNoStop(text)) {
            query_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const set<string>& query_words) const {
        vector<Document> matched_documents;

        for (const DocumentContent& content : _documents) {
            const int relevance = MatchDocument(content, query_words);
            if (relevance > 0) {
                matched_documents.push_back({content.id, relevance});
            }
        }
        return matched_documents;
    }

    static int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
        if (query_words.empty()) {
            return 0;
        }
        set<string> matched_words;
        for (const string& word : content.words) {
            if (matched_words.count(word) != 0) {
                continue;
            }
            if (query_words.count(word) != 0) {
                matched_words.insert(word);
            }
        }
        return static_cast<int>(matched_words.size());
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (_stop_words.count(word) == 0) {
                words.push_back(word);
            }
        }
        return words;
    }

    static vector<string> SplitIntoWords(const string& text) {
        istringstream iss(text);
        string word;
        vector<string> vec_of_words;

        while (iss >> word) {
            vec_of_words.push_back(word);
        }

        return vec_of_words;
    }

    vector<string> SplitIntoWordsYandexVersion(const string& text) {
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

   public:
    static bool sortDocumentsGreaterRelevance(const Document& lhs,
                                              const Document& rhs) {
        // if documents have the same relevance
        if (lhs.relevance == rhs.relevance) {
            return lhs.id > rhs.id;
        } else {
            return lhs.relevance > rhs.relevance;
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        /* // my approach to sort matched_documents
        sort(matched_documents.begin(), matched_documents.end(), sortDocumentsGreaterRelevance);
         */

        /* // Yandex approach to sort matched_documents
        sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance); */

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (lhs.relevance == rhs.relevance) {
                     return lhs.id > rhs.id;  // sord by id in descending order if document relevances are equal
                 } else {
                     return lhs.relevance > rhs.relevance;  // sort by document relevance by default
                 }
             });

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    void AddDocument(int document_id,
                     const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        _documents.push_back({document_id, words});
    }

    void SetStopWords(const string& text) {
        set<string> stop_words;
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    void ComposeDocumentsDB() {
        try {
            const int doc_count = stoi(ReadLine());

            for (int doc_id = 0; doc_id < doc_count; ++doc_id) {
                this->AddDocument(doc_id, ReadLine());
            }
        } catch (std::invalid_argument& e) {
            // if no conversion could be performed
            cout << "Invalid argument for [document_count] variable." << endl;
        } catch (std::out_of_range& e) {
            // if the converted value would fall out of the range of the result type
            // or if the underlying function (std::strtol or std::strtoull) sets errno
            // to ERANGE.
            cout << "[document_count] is out of range." << endl;
        } catch (...) {
            // everything else
            cout << "Unexpected error occured." << endl;
        }
    }
};

// Считывает из cin стоп-слова и документ и возвращает настроенный экземпляр поисковой системы
SearchServer CreateSearchServer() {
    SearchServer srv_instance;
    srv_instance.SetStopWords(ReadLine());
    srv_instance.ComposeDocumentsDB();

    return srv_instance;
}

int main() {
    const SearchServer srv_instance = CreateSearchServer();

    const string query = ReadLine();

    for (auto [document_id, relevance] : srv_instance.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id
             << ", relevance = "s << relevance << " }"s
             << endl;
    }
}