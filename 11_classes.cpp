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
   public:
    void AddDocument(int document_id,
                     const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document, _stop_words);
        _documents.push_back({document_id, words});
    }

    int GetStopWordsSize() {
        return static_cast<int>(_stop_words.size());
    }

   private:
    struct DocumentContent {
        int id;
        vector<string> words;
    };

    vector<DocumentContent> _documents;
    set<string> _stop_words;
};

struct Document {
    int id;
    int relevance;
};


bool HasDocumentGreaterRelevance(const Document& lhs,
                                 const Document& rhs) {
    return lhs.relevance > rhs.relevance;
}

bool sortDocumentsByRelevance(const Document& lhs,
                              const Document& rhs) {
    // if documents have the same relevance
    if (lhs.relevance == rhs.relevance) {
        return lhs.id > rhs.id;
    } else {
        return lhs.relevance > rhs.relevance;
    }
}

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
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

// composes set of stop words
set<string> ParseStopWords(const string& text) {
    set<string> stop_words;

    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text,
                                    const set<string>& stop_words) {
    vector<string> words;

    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}

int MatchDocument(const DocumentContent& db_entry,
                  const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }

    set<string> matched_words;

    for (const string& word : db_entry.words) {
        if (matched_words.count(word) != 0) {
            continue;
        }

        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

void ComposeDocumentsDB(vector<DocumentContent>& documents,
                        const set<string>& stop_words) {
    try {
        const int doc_count = stoi(ReadLine());

        for (int doc_id = 0; doc_id < doc_count; ++doc_id) {
            AddDocument(documents, stop_words, doc_id, ReadLine());
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

// Returns vector of pairs in form of {document_id, document_relevance}
vector<Document> FindAllDocuments(const vector<DocumentContent>& documents_db,
                                  const set<string>& stop_words,
                                  const string& query) {
    const set<string> query_words = ParseQuery(query, stop_words);

    // each pair in vector represents {document_relevance, document_id}
    vector<Document> matched_documents;

    for (const auto& db_entry : documents_db) {
        int doc_id = db_entry.id;
        const int doc_relevance = MatchDocument(db_entry, query_words);

        if (doc_relevance > 0) {
            matched_documents.push_back({doc_id, doc_relevance});
        }
    }
    return matched_documents;
}

// Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
vector<Document> FindTopDocuments(const vector<DocumentContent>& documents_db,
                                  const set<string>& stop_words,
                                  const string& raw_query) {
    vector<Document> ranked_docs = FindAllDocuments(documents_db, stop_words, raw_query);
    sort(ranked_docs.begin(), ranked_docs.end(), HasDocumentGreaterRelevance);

    if (static_cast<int>(ranked_docs.size()) >= MAX_RESULT_DOCUMENT_COUNT) {
        return vector<Document>{ranked_docs.begin(), ranked_docs.begin() + MAX_RESULT_DOCUMENT_COUNT};
    } else {
        return ranked_docs;
    }
}

int main() {
    // Placeholder for documents DB: each entry is represented by pair:
    // {FIRST: document_id; SECOND: vector of words composing the document}
    vector<DocumentContent> documents;

    // get set of unique stop words
    const set<string> stop_words_unique = ParseStopWords(ReadLine());

    // read documents into database
    ComposeDocumentsDB(documents, stop_words_unique);

    // read query, the last line in the input
    const string query = ReadLine();

    for (const auto& [document_id, relevance] : FindTopDocuments(documents, stop_words_unique, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s
             << endl;
    }
}