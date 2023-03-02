#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

void PrintWords(const std::vector<std::string>& v) {
    bool isFirst = true;

    for(const auto& word : v) {
        if (isFirst) {
            std::cout << word;
            isFirst = false;
        } else {
            std::cout << ' ' << word;
        }
    }
    std::cout << std::endl;
}

void ChangeInt(int x) {
    x = 42;
}

int main() {
    std::vector<std::string> v{"air"s, "fire"s, "water"s};
    PrintWords(v);

    int a = 5;
    ChangeInt(a);
    std::cout << a << std::endl;

    return 0;
}
