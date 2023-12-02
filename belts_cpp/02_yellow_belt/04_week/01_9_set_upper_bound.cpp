// CPP program to demonstrate the
// set::upper_bound() function
#include <iostream>
#include <set>
#include <sstream>  // std::ostringstream
#include <string>

using namespace std;

template <typename Collection>
std::string Join(const Collection& collection, const std::string separator = " "s) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }
        isFirst = false;
        oss << elm;
    }

    return oss.str();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

int main() {
    {
        set<int> s = {1, 2, 4, 5, 6};

        cout << "The set elements are: " << s << std::endl;  // {1, 2, 4, 5, 6}

        // when 2 is present, "it" points to next element after 2
        int border = 2;
        auto it = s.upper_bound(border);
        cout << "The upper bound of key "s << border << " is "s << *it << std::endl;

        // when 3 is NOT present, "it" points to next greater after 3, if exists
        border = 3;
        it = s.upper_bound(border);
        cout << "The upper bound of key "s << border << " is "s << *it << std::endl;

        // when 4 is present, "it" points to next element after 4, i.e. to 5
        border = 4;
        it = s.upper_bound(border);
        cout << "The upper bound of key "s << border << " is "s << *it << std::endl;
    }

    std::cout << std::endl
              << "==============================================="s << std::endl;
    {
        set<int> s = {5, 6};
        cout << "The set elements are: " << s << std::endl;  // {5, 6}

        // when 3 is NOT present, "it" points to next greater after 3, i.e. to 5
        int border = 3;
        auto it = s.upper_bound(border);
        cout << "The upper bound of key "s << border << " is "s << *it << std::endl;
    }
    std::cout << std::endl
              << "==============================================="s << std::endl;
    {
        set<int> s = {1, 5, 7, 8};
        cout << "The set elements are: " << s << std::endl;  // {1, 5, 7, 8}

        // when 9 is NOT present, AND there are no any elements greater 9, "it" should point to "s.end()"
        int border = 8;
        auto it = s.upper_bound(border);
        if (it != s.end()) {
            cout << "The upper bound of key "s << border << " is "s << *it << std::endl;
        } else {
            cout << "There are no any elements greater than "s << border << " in set " << s << std::endl;
        }
    }

    return 0;
}
