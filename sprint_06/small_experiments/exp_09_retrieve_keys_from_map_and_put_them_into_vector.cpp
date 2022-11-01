#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (next(it) != endIt) {
            cout << *it << " "s;
        } else {
            cout << *it << endl;
        }
    }
}

int main() {
    std::map<int, std::string> mymap = {
        {12, "hello"s},
        {4, "four"s},
        {18, "world"s}};

    std::vector<int> v;
    for (const auto& [key, value] : mymap) {
        v.push_back(key);
    }

    PrintRange(v.begin(), v.end());

    return 0;
}
