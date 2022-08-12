#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// global constant to store maximum number of relevant documents to retirieve
int MAX_RESULT_DOCUMENT_COUNT = 5;

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

void AddDocument(vector<pair<int, vector<string>>>& documents,
                 const set<string>& stop_words,
                 int document_id, const string& document) {
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back(pair<int, vector<string>>{document_id, words});
}

set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}


// !!!!!!!!!!!!!!! TO DO:
// structured bindings in C++ is analogues to unpacking in Python
// Returns document_relevance.
int MatchDocument(const pair<int, vector<string>>& db_entry,
                  const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }
    set<string> matched_words;
    
    for (const auto& [doc_id, vector_of_word] : db_entry) {
        if (matched_words.count(word) != 0) {
            continue;
        }
        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

void ComposeDocumentsDB(vector<pair<int, vector<string>>>& documents,
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

// Returns vector of pairs in form of {document_relevance, document_id}
vector<pair<int, int>> FindAllDocuments(const vector<pair<int, vector<string>>>& documents,
                                        const set<string>& stop_words,
                                        const string& query) {
    const set<string> query_words = ParseQuery(query, stop_words);

    // each pair in vector represents {document_relevance, document_id}
    vector<pair<int, int>> matched_documents;

    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);

        if (relevance > 0) {
            matched_documents.push_back({relevance, document.first});
        }
    }
    return matched_documents;
}

/*
// Для каждого документа возвращает его релевантность и id
vector<pair<int, int>> FindAllDocuments(const vector<pair<int, vector<string>>>& documents,
    const set<string>& query_words)
{
    // Превратите функцию FindDocuments в FindAllDocuments
    // Первым элементом возвращаемых пар идёт релевантность документа, а вторым - его id
}
*/

/*
// Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
vector<pair<int, int>> FindTopDocuments(const vector<pair<int, vector<string>>>& documents,
                                        const set<string>& stop_words, const string& raw_query) {
    // Напишите функцию, используя FindAllDocuments
}
*/

int main() {
    // Placeholder for documents DB: each entry is represented by pair:
    // {FIRST: document_id; SECOND: vector of words composing the document}
    vector<pair<int, vector<string>>> documents;

    // get set of stop words
    const set<string> stop_words_unique = ParseStopWords(ReadLine());

    // read documents into database
    ComposeDocumentsDB(documents, stop_words_unique);

    // read query, the last line in the input
    const string query = ReadLine();

    // Вместо FindDocuments используйте FindTopDocuments
    for (auto [document_id, relevance] : FindAllDocuments(documents, stop_words_unique, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s
             << endl;
    }
}