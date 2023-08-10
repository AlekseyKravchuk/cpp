#include <iostream>
#include <string>
#include <string_view>

int main() {
    // std::string_view s = "abcd           ";
    std::string_view s = "           ";

    // // remove leading whitespaces
    size_t pos = s.find_first_not_of(" ");
    if (pos != s.npos) {
        s.remove_prefix(pos);
    }

    // remove trailing whitespaces
    pos = s.find_last_not_of(" ");
    if (pos != s.npos) {
        s.remove_suffix(s.size() - (pos + 1));
    } else {
        // строка состоит только из пробельных символов
        s.remove_suffix(s.size());
    }

    std::cout << "s.size() = " << s.size() << std::endl;
    std::cout << "s = " << s << std::endl;

    return 0;
}
