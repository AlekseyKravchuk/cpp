#include "synonyms.h"

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(first_word);

    // // код с преднамеренно внесенной ошибкой
    // synonyms[first_word].insert(second_word);
    // synonyms[second_word].insert(second_word);
}

size_t GetSynonymsCount(Synonyms& synonyms,
                        const std::string& word) {
    return synonyms[word].size();

    // // // код с преднамеренно внесенной ошибкой
    // return synonyms[word].size() + 1;
}

bool AreSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    return synonyms[first_word].count(second_word);

    // // код с преднамеренно внесенной ошибкой
    // return synonyms[first_word].count(second_word) + 1;
}