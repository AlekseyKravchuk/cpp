#include "parse.h"

// remove leading and trailing whitespaces
void trim(std::string_view& s_view) {
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
        s_view.remove_suffix(s_view.size()); // строка состоит только из пробельных символов
    }
}

std::unordered_map<std::string_view, size_t> GetWordsCounterView(std::string_view s_view) {
    trim(s_view); // избавляемся от пробельных символов в начале и в конце string_view
    std::unordered_map<std::string_view, size_t> result;

    while (true) {
        size_t space_pos = s_view.find(' '); // ищем позицию первого пробела в ещё не обработанной части
        ++result[s_view.substr(0, space_pos)];

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') { // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view s_view) {
    trim(s_view); // избавляемся от пробельных символов в начале и в конце строки
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');           // ищем позицию первого пробела в ещё не обработанной части
        result.push_back(s_view.substr(0, space_pos)); // помещаем подстроку-string_view в вектор

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') { // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::string_view Strip(std::string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }

    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }

    return s;
}

std::vector<std::string_view> SplitBy(std::string_view s, char sep) {
    std::vector<std::string_view> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(s.substr(0, pos));
        s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
    }

    return result;
}
