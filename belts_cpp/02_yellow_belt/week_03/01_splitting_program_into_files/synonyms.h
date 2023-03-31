#pragma once

#include <map>
#include <set>
#include <string>

using Synonyms = std::map<std::string, std::set<std::string>>;

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word);

size_t GetSynonymsCount(const Synonyms& synonyms,
                        const std::string& word);

bool AreSynonyms(const Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word);
