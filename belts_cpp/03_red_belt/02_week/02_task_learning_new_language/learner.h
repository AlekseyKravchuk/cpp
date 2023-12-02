#include <set>
#include <string>
#include <vector>

class Learner {
   public:
    int Learn(const std::vector<std::string>& words);
    std::vector<std::string> KnownWords();

   private:
    std::set<std::string> _dict;
};
