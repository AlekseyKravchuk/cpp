#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

std::vector<std::string> SplitIntoWords(const std::string& str);
std::vector<std::string> SplitIntoWords_v2(const std::string& str);
std::vector<std::string> SplitIntoWords_via_istream_iterator(const std::string& str);

std::vector<std::string_view> SplitIntoWordsView_v1(std::string_view view);
std::vector<std::string_view> SplitIntoWordsView_v2(std::string_view view);
std::vector<std::string_view> SplitIntoWordsView_v3(std::string_view view);


