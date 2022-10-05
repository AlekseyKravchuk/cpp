#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

ostream& operator<<(ostream& os, const Lang& language) {
    os << language.name << ", " << language.age;
    return os;
}

    int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    // Выведите первый язык, начинающийся на J, используя алгоритм find_if
    const auto it = find_if(langs.begin(), langs.end(),
                            [](const Lang& language) {
                                return language.name[0] == 'J';
                            });
    cout << *it << endl;
}