#include "stats.h"

void Stats::AddMethod(std::string_view method) {
    if (SUPPORTED_METHODS.count(method)) {
        ++_method_stats[method];
    } else {
        ++_method_stats["UNKNOWN"];
    }
}

void Stats::AddUri(std::string_view uri) {
    if (SUPPORTED_URIs.count(uri)) {
        ++_uri_stats[uri];
    } else {
        ++_uri_stats["unknown"];
    }
}

const std::map<std::string_view, int>& Stats::GetMethodStats() const {
    return _method_stats;
}

const std::map<std::string_view, int>& Stats::GetUriStats() const {
    return _uri_stats;
}

std::vector<std::string_view> SplitIntoWords(std::string_view s_view) {
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');
        result.push_back(s_view.substr(0, space_pos));

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок
            s_view.remove_prefix(space_pos);  // в качестве параметра указываем длину префикса, который нужно откусить
        }
    }

    return result;
}

// remove leading and trailing whitespaces
void trim(std::string_view& s) {
    // remove leading whitespaces
    size_t pos = s.find_first_not_of(" ");
    if (pos != s.npos) {
        s.remove_prefix(pos);
    }

    // remove trailing whitespaces
    pos = s.find_last_not_of(" ");
    if (pos != s.npos) {
        s.remove_suffix(s.size() - (pos + 1));
    } else {
        s.remove_suffix(s.size());  // строка состоит только из пробельных символов
    }
}

HttpRequest ParseRequest(std::string_view line) {
    trim(line);
    std::vector<std::string_view> words = SplitIntoWords(line);
    std::string_view method{}, uri{}, protocol{};

    if (SUPPORTED_METHODS.count(words[0])) {
        method = *SUPPORTED_METHODS.find(words[0]);
    } else {
        UNSUPPORTED_STORAGE.insert(std::string(words[0]));
        method = *UNSUPPORTED_STORAGE.find(std::string(words[0]));
    }

    if (SUPPORTED_URIs.count(words[1])) {
        uri = *SUPPORTED_URIs.find(words[1]);
    } else {
        UNSUPPORTED_STORAGE.insert(std::string(words[1]));
        uri = *UNSUPPORTED_STORAGE.find(std::string(words[1]));
    }

    if (SUPPORTED_PROTOCOLS.count(words[2])) {
        protocol = *SUPPORTED_PROTOCOLS.find(words[2]);
    } else {
        UNSUPPORTED_STORAGE.insert(std::string(words[2]));
        protocol = *UNSUPPORTED_STORAGE.find(std::string(words[2]));
    }

    return {method, uri, protocol};
}
