#include "synonyms.h"

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(first_word);
}

size_t GetSynonymsCount(const Synonyms& synonyms,
                        const std::string& word) {
    return (synonyms.count(word)) ? synonyms.at(word).size() : 0;
}

// функция проверяет, являются ли 2 слова синонимами
bool AreSynonyms(const Synonyms& synonyms,
                 const std::string& first_word, const std::string& second_word) {
    // корректный код
    if (synonyms.count(first_word)) {
        if (synonyms.at(first_word).count(second_word)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
