#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

int main() {
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s};

    // хотим вывести содержимое вектора "langs" в обратном порядке
    auto it = langs.end();
    while (it != langs.begin()) {
        --it;
        std::cout << *it << " "s;
    }
    
    std::cout << std::endl;

    return 0;
}
