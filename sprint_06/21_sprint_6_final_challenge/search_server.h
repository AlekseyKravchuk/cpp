#pragma once
//#ifndef __SEARCH_SERVER__H__
//#define __SEARCH_SERVER__H__

#include <algorithm>  // std::all_of
#include <map>        // std::map
#include <numeric>    // std::reduce
#include <set>        // std::set
#include <stdexcept>  // std::invalid_argument, std::out_of_range
#include <string>     // std::string
#include <tuple>      // std::tuple
#include <vector>     // std::vector

#include "document.h"
#include "string_processing.h"

//using namespace std::literals;
using namespace std;

class SearchServer {
   public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stopWords);

    // Invoke delegating constructor from string container: SearchServer(SplitIntoWords(stop_words_text))
    explicit SearchServer(const std::string& stopWordsRawText)
        : SearchServer(SplitIntoWords(stopWordsRawText)) {}

    int GetDocumentCount() const;

    // Document will be added in all cases except:
    // 1) docID < 0;
    // 2) document with given docID already exists in "_documents" data base
    // 3) if at least one word in the document contains special character
    void AddDocument(int docID,
                     const std::string& rawDocument,
                     DocumentStatus status,
                     const std::vector<int>& ratings);

    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& rawQuery,
                                           DocumentPredicate documentPredicate) const;

    // #2) FindTopDocuments WRAPPER: converts STATUS-based to Predicate-based logic
    std::vector<Document> FindTopDocuments(const std::string& rawQuery,
                                           DocumentStatus status) const;

    // #3) FindTopDocuments WRAPPER: check for ACTUAL status by default
    std::vector<Document> FindTopDocuments(const std::string& rawQuery) const;

    // Returns tuple: First component is a vector of PLUS-query-words found in document specified by docID
    //                Second component is STATUS of document specified by docID
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& rawQuery, int docID) const;

    std::set<int>::const_iterator begin() const;
    std::set<int>::const_iterator end() const;

    //  метод получения частот слов по id документа
    const map<string, double>& GetWordFrequencies(int docID) const;

    // метод удаления документов из поискового сервера
    void RemoveDocument(int document_id);

    // функция для поиска и удаления дубликатов
    friend void RemoveDuplicates(SearchServer& search_server);

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool isStopWord;
    };

    struct Query {
        std::set<std::string> plusWords;
        std::set<std::string> minusWords;
    };

    std::set<std::string> _stopWords;
    std::map<std::string, std::map<int, double>> _word_docID_freqs;
    std::map<int, std::map<std::string, double>> _docID_words_freqs;

    std::map<int, DocumentData> _documents;
    std::set<int> _docsIdentifiers;

    // returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
    static bool IsWordWithoutSpecialChars(const std::string word2Check);

    void SetStopWords(const std::string& text);

    static bool IsValidWord(const std::string& word);

    bool IsStopWord(const std::string& word) const;

    // returns vector of words composing the document content (excluding stop-words)
    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;

    static int ComputeAverageRating(const std::vector<int>& ratings);

    QueryWord ParseQueryWord(std::string queryWord) const;

    Query ParseQuery(const std::string& rawQuery) const;

    // Existence required
    double ComputeInvertedDocumentFreq(const std::string& word) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query,
                                           DocumentPredicate documentPredicate) const;
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stopWords) : _stopWords(MakeUniqueNonEmptyStrings(stopWords)) {
    // check stop words whether they contain special characters
    for (auto& stopWord : stopWords) {
        if (!IsValidWord(stopWord)) {
            std::string exception_message = "stop word: \""s + stopWord + "\" is not valid"s;
            throw std::invalid_argument(exception_message);
        }
    }
}

// #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string& rawQuery,
                                                     DocumentPredicate documentPredicate) const {
    std::vector<Document> foundDocs;
    Query parsedQuery = ParseQuery(rawQuery);

    if (parsedQuery.plusWords.empty()) {
        return foundDocs;
    }

    foundDocs = FindAllDocuments(parsedQuery, documentPredicate);
    if (!foundDocs.empty()) {
        sort(foundDocs.begin(), foundDocs.end(),
             [](const Document& lhs, const Document& rhs) {
                 double reasonable_error = 1e-6;
                 if (abs(lhs.relevance - rhs.relevance) < reasonable_error) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
    }

    if (foundDocs.size() > MAX_RESULT_DOCUMENT_COUNT) {
        foundDocs.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    return foundDocs;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query,
                                                     DocumentPredicate documentPredicate) const {
    std::map<int, double> doc2Relevance;

    for (const std::string& word : query.plusWords) {
        if (_word_docID_freqs.count(word) == 0) {
            continue;
        }

        const double inverseDocFrequency = ComputeInvertedDocumentFreq(word);
        for (const auto [docID, termFreq] : _word_docID_freqs.at(word)) {
            const auto& documentData = _documents.at(docID);
            if (documentPredicate(docID, documentData.status, documentData.rating)) {
                doc2Relevance[docID] += termFreq * inverseDocFrequency;
            }
        }
    }

    for (const std::string& minusWord : query.minusWords) {
        // continue if given MINUS-word was NOT found among all the documents content
        if (_word_docID_freqs.count(minusWord) == 0) {
            continue;
        }

        // exclude from search result all the documents containing MINUS-words
        for (const auto [docID, _] : _word_docID_freqs.at(minusWord)) {
            doc2Relevance.erase(docID);
        }
    }

    std::vector<Document> foundDocs;
    for (const auto [docID, relevance] : doc2Relevance) {
        foundDocs.push_back(
            {docID, relevance, _documents.at(docID).rating});
    }
    return foundDocs;
}

//#endif  //!__SEARCH_SERVER__H__
