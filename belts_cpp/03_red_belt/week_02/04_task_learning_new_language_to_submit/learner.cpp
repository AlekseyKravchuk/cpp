#include <algorithm>
#include <set>
#include <string>
#include <vector>

class Learner {
   private:
    std::set<std::string> _dict;

   public:
    int Learn(const std::vector<std::string>& words) {
        int initial_dict_size = static_cast<int>(_dict.size());
        for (const auto& word : words) {
            if (!_dict.count(word)) {
                _dict.insert(word);
            }
        }
        return static_cast<int>(_dict.size()) - initial_dict_size;
    }

    std::vector<std::string> KnownWords() {
        return {_dict.begin(), _dict.end()};
    }
};
