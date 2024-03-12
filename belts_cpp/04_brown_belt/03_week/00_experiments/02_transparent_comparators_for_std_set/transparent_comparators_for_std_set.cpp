#include <iomanip>
#include <iostream>
#include <set>
#include <string>

using namespace std;

struct MyData {
    string key;
    string value2;
};

struct Compare {
    bool operator()(const MyData& lhs, const MyData& rhs) const {
        return lhs.key < rhs.key;
    }

    template <typename T>
    bool operator()(const MyData& lhs, const T& rhs) const {
        return lhs.key < rhs;
    }

    template <typename T>
    bool operator()(const T& lhs, const MyData& rhs) const {
        return lhs < rhs.key;
    }

    // using is_transparent = void;
    using is_transparent = std::false_type;
};

int main() {
    set<MyData, Compare> myset;
    myset.insert(MyData{"Bob", {}});

    // хочется, чтобы была возможность осуществлять поиск по ключу
    cout << boolalpha << (myset.count("Bob") > 0) << endl;

    // cout << boolalpha << (myset.count(MyData{"Bob", {}}) > 0) << endl;

    return 0;
}
