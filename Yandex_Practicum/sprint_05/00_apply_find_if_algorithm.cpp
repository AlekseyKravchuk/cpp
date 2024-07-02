#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};

    // Выведите первый язык, начинающийся на C, используя алгоритм find_if
    const auto it = std::find_if(langs.begin(), langs.end(), [](string s) {
        return s[0] == 'C';
        // return s[0] == 'Z';
    });

    cout << ((it != langs.end()) ? *it : "The element you are looking for does not exist in the container."s) << endl;

}