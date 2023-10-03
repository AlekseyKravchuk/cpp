#include <iostream>
#include <istream>
#include <iterator>  // std::istream_iterator
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "print.h"

// class InvertedIndex {
//    public:
//     void Add(const std::string& document);

//     std::list<size_t> Lookup(const std::string& word) const {
//         // TODO: just method stub for testing
//         return {};
//     }

//     const std::string& GetDocument(size_t id) const {
//         return _docs[id];
//     }

//    private:
//     std::map<std::string, std::list<size_t>> _index;
//     std::vector<std::string> _docs;
// };

// class SearchServer {
//    public:
//     SearchServer() = default;
//     explicit SearchServer(std::istream& document_input);
//     void UpdateDocumentBase(std::istream& document_input);
//     void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

//    private:
//     InvertedIndex _inv_index_inst;
// };

// std::vector<std::string> SplitIntoWords(const std::string& line) {
//     std::istringstream words_input(line);

//     return {std::istream_iterator<std::string>(words_input),
//             std::istream_iterator<std::string>()};
// }

std::vector<std::string_view> SplitIntoWords(std::string_view s_view) {
    // get rid of leading and trailing whitespaces using unnamed lambda
    [&s_view]() {
        // remove leading whitespaces
        size_t pos = s_view.find_first_not_of(" ");
        if (pos != s_view.npos) {
            s_view.remove_prefix(pos);
        }

        // remove trailing whitespaces
        pos = s_view.find_last_not_of(" ");
        if (pos != s_view.npos) {
            s_view.remove_suffix(s_view.size() - (pos + 1));
        } else {
            s_view.remove_suffix(s_view.size());  // строка состоит только из пробельных символов
        }
    }();

    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');            // ищем позицию первого пробела в ещё не обработанной части
        result.push_back(s_view.substr(0, space_pos));  // помещаем подстроку-string_view в вектор

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {  // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

int main() {
    std::string line = "    a small      curly     guinea pig     with grey hair has been found     ";
    // std::vector<std::string> strings;
    std::vector<std::string_view> s_views;

    // strings = SplitIntoWords(line);
    s_views = SplitIntoWords(line);
    std::cout << s_views << std::endl;

    return 0;
}
