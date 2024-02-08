#include <functional>
#include <iostream>
#include <string>
#include <utility>  // std::move, std::as_const

#include "print.h"

using namespace std;

int main() {
    vector<string> words = {"toy", "zinger", "abacaba", "abcde", "bamby", "girl", "mouse"};

    std::sort(words.begin(), words.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs[0] < rhs[0];
              });
    
    cout << words << endl;

    return 0;
}
