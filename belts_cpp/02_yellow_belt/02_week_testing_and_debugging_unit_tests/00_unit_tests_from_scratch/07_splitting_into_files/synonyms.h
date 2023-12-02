#pragma once
#include <string>
#include <map>
#include <set>

using Synonyms = std::map<std::string, std::set<std::string>>;

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word);

size_t GetSynonymsCount(Synonyms& synonyms,
                        const std::string& word);

bool AreSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word);