#include "learner.h"

#include <algorithm>

int Learner::Learn(const std::vector<std::string>& words) {
    int initial_dict_size = static_cast<int>(_dict.size());
    for (const auto& word : words) {
        if (!_dict.count(word)) {
            _dict.insert(word);
        }
    }
    return static_cast<int>(_dict.size()) - initial_dict_size;
}

std::vector<std::string> Learner::KnownWords() {
    return {_dict.begin(), _dict.end()};
}
