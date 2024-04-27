#include <iostream>
#include <string_view>

using namespace std;

string_view ltrim_view(string_view s) {
    s.remove_prefix(std::min(s.find_first_not_of(' '), s.size()));
    return s;
}

std::string_view rtrim_view(std::string_view s) {
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(' ') - 1, s.size()));
    return s;
}

std::string_view trim_view(std::string_view s) {
    return ltrim_view(rtrim_view(s));
}

int main() {
    string info = "Tolstopaltsevo: 55.611087, 37.20829";
    string_view s{info};
    char sep = ':';
    size_t pos = s.find(sep);

    string_view first = trim_view(s.substr(0, pos));
    string_view second = trim_view(s.substr(pos+1));

    std::cout << "first = "  << first  << endl
              << "second = " << second << endl;
    return 0;
}
