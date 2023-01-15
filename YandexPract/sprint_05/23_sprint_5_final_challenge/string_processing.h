#pragma once
//#ifndef __STRING_PROCESSING__H__
//#define __STRING_PROCESSING__H__

#include <iostream>
#include <set>
#include <string>
#include <vector>

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    std::set<std::string> nonEmptyStrings;

    for (const std::string& str : strings) {
        if (!str.empty()) {
            nonEmptyStrings.insert(str);
        }
    }

    return nonEmptyStrings;
}

std::vector<std::string> SplitIntoWords(const std::string& text);

//#endif  //!__STRING_PROCESSING__H__
