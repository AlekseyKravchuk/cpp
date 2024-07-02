#include "remove_duplicates.h"

// функция для поиска и удаления дубликатов
void RemoveDuplicates(SearchServer& searchSrv) {
    std::set<std::set<std::string>> docsAsSetsOfUniqueWords;
    std::vector<int> docIDsToDelete;

    // ================ формирование "docsAsSetsOfUniqueWords" ================
    for (const int docID : searchSrv) {
        std::set<std::string> docWords;

        // восстанавливаем множество слов, из которых состоит текущий документ
        for (auto& [word, _] : searchSrv._docID_words_freqs[docID]) {
            docWords.insert(word);
        }

        if (docsAsSetsOfUniqueWords.count(docWords)) {
            // найден дубль, фиксируем его docID
            docIDsToDelete.push_back(docID);
        } else {
            // документ уникален => добавляем его в "docsAsSetsOfUniqueWords"
            docsAsSetsOfUniqueWords.insert(docWords);
        }
    }
    // =========== ОКОНЧАНИЕ формирования "docsAsSetsOfUniqueWords" ===========

    for (auto docID2Delete : docIDsToDelete) {
        std::cout << "Found duplicate document id " << docID2Delete << std::endl;
        searchSrv.RemoveDocument(docID2Delete);
    }
}