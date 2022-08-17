#include <algorithm>  // header for function "std::sort()"
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// global constant to store maximum number of relevant documents to retirieve
int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer {
   private:
    struct RelevanceInfo {
        int id;
        int relevance;
    };

    struct DocumentContent {
        int id;
        vector<string> words;
    };

    // class attributes (data members)
    vector<DocumentContent> _documents;
    vector<RelevanceInfo> _ranked;
    set<string> _stop_words;
    set<string> _query_words_no_stop_words;

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

   public:
    SearchServer() {
        ParseStopWords(ReadLine());
        ComposeDocumentsDB();
    }

    static bool HasDocumentGreaterRelevance(const RelevanceInfo& lhs,
                                            const RelevanceInfo& rhs) {
        return lhs.relevance > rhs.relevance;
    }

    static bool sortDocumentsGreaterRelevance(const RelevanceInfo& lhs,
                                              const RelevanceInfo& rhs) {
        // if documents have the same relevance
        if (lhs.relevance == rhs.relevance) {
            return lhs.id > rhs.id;
        } else {
            return lhs.relevance > rhs.relevance;
        }
    }

    void processQueries() {
        FindTopDocuments();
        for (const RelevanceInfo& obj : _ranked) {
            cout << "{ document_id = "s << obj.id << ", relevance = "s << obj.relevance << " }"s
                 << endl;
        }
    }

    void ParseQuery(const string& text) {
        for (const string& word : SplitIntoWordsNoStop(text)) {
            _query_words_no_stop_words.insert(word);
        }
    }

    // composes set of stop words
    void ParseStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    vector<string> SplitIntoWordsNoStop(const string& text) {
        vector<string> vecOfWords;

        for (const string& word : SplitIntoWords(text)) {
            if (_stop_words.count(word) == 0) {
                vecOfWords.push_back(word);
            }
        }

        return vecOfWords;
    }

    // Returns vector of pairs in form of {document_id, document_relevance}
    void FindAllDocuments() {
        // each element in "_relevance_info" vector is struct:
        // Document{document_id, document_relevance}

        for (const DocumentContent& obj : _documents) {
            const int doc_relevance = MatchDocument(obj.words);

            if (doc_relevance > 0) {
                _ranked.push_back({obj.id, doc_relevance});
            }
        }
    }

    // Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
    void FindTopDocuments() {
        FindAllDocuments();
        sort(_ranked.begin(), _ranked.end(), SearchServer::sortDocumentsGreaterRelevance);

        if (static_cast<int>(_ranked.size()) > MAX_RESULT_DOCUMENT_COUNT) {
            _ranked.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
    }

    void AddDocument(int document_id,
                     const string& document) {
        _documents.push_back({document_id, SplitIntoWordsNoStop(document)});
    }

    int MatchDocument(vector<string> doc_words) {
        if (_query_words_no_stop_words.empty()) {
            return 0;
        }

        set<string> matched_words;

        for (const string& word : doc_words) {
            if (matched_words.count(word) != 0) {
                continue;
            }

            if (_query_words_no_stop_words.count(word) != 0) {
                matched_words.insert(word);
            }
        }
        return static_cast<int>(matched_words.size());
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

    string ReadLine() {
        string s;
        getline(cin, s);
        return s;
    }
};

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int main() {
    SearchServer srv_instance;

    // read query, the last line in the input
    string query = ReadLine();
    srv_instance.ParseQuery(query);

    srv_instance.processQueries();
}