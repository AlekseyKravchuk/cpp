#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>  // for std::reduce()
#include <set>
#include <sstream>  // for std::istringstream()
#include <stdexcept>
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
    int result;
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
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
   public:
    int GetDocumentCount() const {
        return static_cast<int>(_documents.size());
    }

    DocumentStatus GetDocumentStatus(int docID) const {
        if (_documents.count(docID) != 0) {
            return _documents.at(docID).status;
        } else {
            throw std::invalid_argument("Received document with non-existing");
        }
    }

    bool ContainsMinusWord(int docID, const set<string>& minusWords) const {
        for (const auto& minusWord : minusWords) {
            if (_word_docID_freqs.count(minusWord) != 0) {
                if (_word_docID_freqs.at(minusWord).count(docID) != 0) {
                    return true;
                }
            }
        }
        return false;
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int docID) const {
        Query queryWords = ParseQuery(raw_query);
        vector<string> result_words;

        if (ContainsMinusWord(docID, queryWords.minus_words)) {
            return tuple(vector<string>(), GetDocumentStatus(docID));
        }

        for (const auto& plusWord : queryWords.plus_words) {
            if (_word_docID_freqs.count(plusWord) != 0) {
                if (_word_docID_freqs.at(plusWord).count(docID) != 0) {
                    result_words.push_back(plusWord);
                }
            }
        }

        return tuple(result_words, GetDocumentStatus(docID));
    }

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            _word_docID_freqs[word][document_id] += inv_word_count;
        }
        _documents.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }

    vector<Document> FindTopDocuments(const string& raw_query,
                                      DocumentStatus status = DocumentStatus::ACTUAL) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, status);

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
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    set<string> _stop_words;
    map<string, map<int, double>> _word_docID_freqs;
    map<int, DocumentData> _documents;

    bool IsStopWord(const string& word) const {
        return _stop_words.count(word) > 0;
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

    static int ComputeAverageRating(const vector<int>& v) {
        return (v.empty()) ? 0 : std::reduce(v.begin(), v.end(), 0, std::plus()) / static_cast<double>(v.size());
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

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(_documents.size() * 1.0 / _word_docID_freqs.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query, DocumentStatus status) const {
        map<int, double> doc2relevance;
        for (const string& word : query.plus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }

            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);

            for (const auto [document_id, term_freq] : _word_docID_freqs.at(word)) {
                if (_documents.at(document_id).status == status) {
                    doc2relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word_docID_freqs.at(word)) {
                doc2relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : doc2relevance) {
            matched_documents.push_back(
                {document_id, relevance, _documents.at(document_id).rating});
        }
        return matched_documents;
    }
};

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) {
    cout << "{ "s
         << "document_id = "s << document_id << ", "s
         << "status = "s << static_cast<int>(status) << ", "s
         << "words ="s;
    for (const string& word : words) {
        cout << ' ' << word;
    }
    cout << "}"s << endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);
    search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, {9});
    const int document_count = search_server.GetDocumentCount();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        const auto [words, status] = search_server.MatchDocument("пушистый кот"s, document_id);
        PrintMatchDocumentResult(document_id, words, status);
    }
}