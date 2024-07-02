#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

int main() {
    const string text = "hello, world"s;
    std::list<char> lst;
    auto it = lst.begin();

    for (char ch : text) {
        lst.insert(it, ch);
    }

    std::cout << "Finished." << std::endl;
}