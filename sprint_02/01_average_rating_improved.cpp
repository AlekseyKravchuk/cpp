#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>  //for reduce() function
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
    int rating;
};

// ==================================================

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

vector<int> GetVectorOfRatings(const string& s) {
    istringstream iss(s);
    int numOfRatings;

    iss >> numOfRatings;
    vector<int> ratings(numOfRatings);

    for (int i = 0; i < numOfRatings; ++i) {
        iss >> ratings[i];
    }

    return ratings;
}

class SearchServer {
   public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    void AddDocument(int docID,
                     const string& rawDocAsString,
                     const vector<int> ratings) {
        ++_document_count;
        const vector<string> wordsNoStop = SplitIntoWordsNoStop(rawDocAsString);
        const double inv_word_count = 1.0 / wordsNoStop.size();

        for (const string& word : wordsNoStop) {
            _word_docID_freqs[word][docID] += inv_word_count;
        }

        _doc_ratings[docID] = ComputeAverageRating(ratings);
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

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
    int _document_count = 0;
    set<string> _stop_words;
    map<string, map<int, double>> _word_docID_freqs;
    map<int, int> _doc_ratings;  // key: document_ID; value: document_Rating

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    bool IsStopWord(const string& word) const {
        return _stop_words.count(word) > 0;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        return reduce(ratings.begin(), ratings.end()) / static_cast<int>(ratings.size());
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

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }

        return {text, is_minus, IsStopWord(text)};
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }

        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(_document_count * 1.0 / _word_docID_freqs.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query) const {
        map<int, double> doc_to_relevance;

        for (const string& word : query.plus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : _word_docID_freqs.at(word)) {
                doc_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }

        for (const string& word : query.minus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }

            for (const auto [document_id, _] : _word_docID_freqs.at(word)) {
                doc_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [docID, relevance] : doc_to_relevance) {
            matched_documents.push_back({docID, relevance, _doc_ratings.at(docID)});
        }
        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        string rawDocAsString = ReadLine();
        vector<int> ratings = GetVectorOfRatings(ReadLine());
        search_server.AddDocument(document_id, rawDocAsString, ratings);
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (auto [docID, relevance, rating] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << docID << ", "s
             << "relevance = "s << relevance << ", "s
             << "rating = "s << rating << " }"s << endl;
    }
}