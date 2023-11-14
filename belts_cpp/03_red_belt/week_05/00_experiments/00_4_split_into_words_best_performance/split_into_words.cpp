#include <sstream>

#include "split_into_words.h"

std::vector<std::string> SplitIntoWords(const std::string& str) {
    std::vector<std::string> result;
    auto str_begin = std::begin(str);
    const auto str_end = std::end(str);

    while (true) {
        auto it = std::find(str_begin, str_end, ' ');

        if (it != str_begin) {
            result.emplace_back(str_begin, it);
        }

        if (it == str_end) {
            break;
        } else {
            while (*it == ' ') {
                ++it;
            }
        }
        str_begin = it;
    }

    return result;
}

std::vector<std::string> SplitIntoWords_via_istream_iterator(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());

    return results;
}

std::vector<std::string_view> SplitIntoWordsView_v1(std::string_view view) {
    auto trim_left = [&view]() {
        size_t first_nonspace_pos = view.find_first_not_of(' ');
        view.remove_prefix(first_nonspace_pos != std::string_view::npos
                               ? first_nonspace_pos
                               : view.size());
    };

    std::vector<std::string_view> result;

    while (true) {
        trim_left();

        if (!view.size()) {
            break;
        }

        size_t space_pos = view.find(' ');
        result.push_back(view.substr(0, space_pos));

        if (space_pos == std::string::npos) {
            break;
        } else {
            view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsView_v2(std::string_view view) {
    std::vector<std::string_view> result;

    while (true) {
        if (view.size() == 0) {
            break;
        }

        size_t space_pos = view.find(' ');
        if (space_pos != 0) {
            result.push_back(view.substr(0, space_pos));
        }

        if (space_pos == std::string::npos) {
            break;
        } else {
            while (view[space_pos] == ' ') {
                ++space_pos;
            }
            view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsView_v3(std::string_view view) {
    std::vector<std::string_view> result;
    size_t first_nonspace_pos = view.find_first_not_of(' ', 0);

    if (first_nonspace_pos == std::string_view::npos) {
        return result;
    }

    while (true) {
        size_t space_pos = view.find(' ', first_nonspace_pos);
        result.emplace_back(view.substr(first_nonspace_pos, space_pos - first_nonspace_pos));

        if (space_pos == view.npos) {
            break;
        } else {
            first_nonspace_pos = view.find_first_not_of(' ', space_pos);
        }

        if (first_nonspace_pos == std::string_view::npos) {
            break;
        }
    }

    return result;
}