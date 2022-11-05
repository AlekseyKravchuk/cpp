#include "search_server.h"

int SearchServer::GetDocumentCount() const {
    return _documents.size();
}

// Document will be added in all cases except:
// 1) docID < 0;
// 2) document with given docID already exists in "_documents" data base
// 3) if at least one word in the document contains special character
void SearchServer::AddDocument(int docID,
                               const std::string& rawDocument,
                               DocumentStatus status,
                               const std::vector<int>& ratings) {
    if (docID < 0) {
        throw std::invalid_argument("passed document ID is negative"s);
    }

    bool documentAlreadyExists = _documents.find(docID) != _documents.end();
    if (documentAlreadyExists) {
        std::string exceptionMessage = "document with ID = "s + std::to_string(docID) + " already exists"s;
        throw std::invalid_argument(exceptionMessage);
    }

    std::vector<std::string> wordsNoStop = SplitIntoWordsNoStop(rawDocument);

    if (!std::all_of(wordsNoStop.begin(), wordsNoStop.end(), IsValidWord)) {
        std::string exceptionMessage = "document with ID = "s + std::to_string(docID) + " contains invalid characters"s;
        throw std::invalid_argument(exceptionMessage);
    }

    const double invertedWordCount = 1.0 / wordsNoStop.size();
    std::set<string> uniqueWords;
    std::string strToBeHashed;

    for (const std::string& word : wordsNoStop) {
        uniqueWords.insert(word);
        _word_docID_freqs[word][docID] += invertedWordCount;
        _docID_words_freqs[docID][word] = _word_docID_freqs[word][docID];
    }

    for (const auto& uniqueWord : uniqueWords) {
        strToBeHashed += uniqueWord;
    }
    size_t strHash = std::hash<std::string>()(strToBeHashed);

    _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
    _hash_docID.emplace(strHash, docID);
    _docsIdentifiers.insert(docID);
}

// #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
std::vector<Document> SearchServer::FindTopDocuments(const std::string& rawQuery,
                                                     DocumentStatus status) const {
    return SearchServer::FindTopDocuments(rawQuery,
                                          [status](int document_id, DocumentStatus documentStatus, int rating) {
                                              return documentStatus == status;
                                          });
}

// #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
std::vector<Document> SearchServer::FindTopDocuments(const std::string& rawQuery) const {
    return SearchServer::FindTopDocuments(rawQuery, DocumentStatus::ACTUAL);
}

// Returns tuple: First component is a vector of PLUS-query-words found in document specified by docID
//                Second component is STATUS of document specified by docID
std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& rawQuery, int docID) const {
    std::vector<std::string> intersectionWithPlusWords;
    DocumentStatus status;

    if (_documents.count(docID)) {
        status = _documents.at(docID).status;
    } else {  // return empty tuple if document with "docID" isn't found
        return {intersectionWithPlusWords, status};
    }

    Query parsedQuery = SearchServer::ParseQuery(rawQuery);

    // return empty tuple if PLUS-words set of a query is empty
    if (parsedQuery.plusWords.empty()) {
        return {intersectionWithPlusWords, status};
    }

    // processing of MINUS-words: if any are found, return empty pair
    for (const std::string& minusWord : parsedQuery.minusWords) {
        // if "minusWord" is found in inverted index
        if (_word_docID_freqs.count(minusWord)) {
            // if "docID" key is found in map "docID : frequency"
            if (_word_docID_freqs.at(minusWord).count(docID)) {
                return {intersectionWithPlusWords, status};
            }
        }
    }

    // processing of PLUS-words
    for (const std::string& plusWord : parsedQuery.plusWords) {
        if (_word_docID_freqs.count(plusWord)) {
            if (_word_docID_freqs.at(plusWord).count(docID)) {
                if (!IsStopWord(plusWord)) {
                    intersectionWithPlusWords.push_back(plusWord);
                }
            }
        }
    }

    return {intersectionWithPlusWords, status};
}

std::set<int>::const_iterator SearchServer::begin() const {
    return _docsIdentifiers.begin();
}

std::set<int>::const_iterator SearchServer::end() const {
    return _docsIdentifiers.end();
}

//  метод получения частот слов по id документа
const map<string, double>& SearchServer::GetWordFrequencies(int docID) const {
    if (_docID_words_freqs.count(docID)) {
        return _docID_words_freqs.at(docID);
    } else {
        static const std::map<string, double> emptyMap;
        return emptyMap;
    }
}

// метод удаления документов из поискового сервера
void SearchServer::RemoveDocument(int docID) {
    if (!_docsIdentifiers.count(docID)) {
        return;
    }

    // восстанавливаем информацию о том, из каких слов состоит документ
    std::vector<std::string> wordsToCheck;
    for (const auto& [word, _] : _docID_words_freqs[docID]) {
        wordsToCheck.push_back(word);
    }

    // удаляем документ с данным "docID"
    _docID_words_freqs.erase(docID);

    // для каждого ключа-слова из "_word_docID_freqs" удаляем записи, содержащие удаляемый "docID" в качестве ключа
    for (const auto& word : wordsToCheck) {
        _word_docID_freqs[word].erase(docID);
    }

    // удаляем идентификатор документа
    _docsIdentifiers.erase(docID);

    // и, наконец, удаляем из "_documents"
    _documents.erase(docID);
}

// returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
bool SearchServer::IsWordWithoutSpecialChars(const std::string word2Check) {
    return std::none_of(word2Check.begin(),
                        word2Check.end(),
                        [](char ch) { return ch >= '\0' && ch < ' '; });
}

void SearchServer::SetStopWords(const std::string& text) {
    for (const std::string& word : SplitIntoWords(text)) {
        _stopWords.insert(word);
    }
}

bool SearchServer::IsValidWord(const std::string& word) {
    // check for multiple '-' in the beginning of a word and for a word consisting from only one '-'
    // minuses in the middle of a word are allowed, for example: "иван-чай", "-иван-чай"
    if (word[0] == '-') {
        if ((word.size() == 1) || (word[1] == '-')) {
            return false;
        }
    }

    // check for "-" at the end of a word
    if (word.back() == '-') {
        return false;
    }

    // final check for the absence of special characters
    return IsWordWithoutSpecialChars(word);
}

bool SearchServer::IsStopWord(const std::string& word) const {
    return _stopWords.count(word) > 0;
}

// returns vector of words composing the document content (excluding stop-words)
std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
    std::vector<std::string> words;

    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }

    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
    return (ratings.empty()) ? 0 : std::reduce(ratings.begin(), ratings.end(), 0.0) / static_cast<double>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string queryWord) const {
    if (queryWord.empty()) {
        std::string exceptionMessage = "query word: \""s + queryWord + "\" is empty"s;
        throw std::invalid_argument(exceptionMessage);
    }

    if (!SearchServer::IsValidWord(queryWord)) {
        std::string exception_message = "query word: \""s + queryWord + "\" is not valid"s;
        throw std::invalid_argument(exception_message);
    }

    bool isMinus = false;
    if (queryWord[0] == '-') {
        isMinus = true;
        queryWord = queryWord.substr(1);
    }
    return {queryWord, isMinus, SearchServer::IsStopWord(queryWord)};
}

SearchServer::Query SearchServer::ParseQuery(const std::string& rawQuery) const {
    SearchServer::Query parsedQuery;

    for (const std::string& word : SplitIntoWords(rawQuery)) {
        const QueryWord queryWord = ParseQueryWord(word);

        if (queryWord.is_minus) {
            parsedQuery.minusWords.insert(queryWord.data);
        } else {
            parsedQuery.plusWords.insert(queryWord.data);
        }
    }
    return parsedQuery;
}

// Existence required
double SearchServer::ComputeInvertedDocumentFreq(const std::string& word) const {
    return log(SearchServer::GetDocumentCount() * 1.0 / _word_docID_freqs.at(word).size());
}
