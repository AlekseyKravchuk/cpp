#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// map<string, int> ComputeTermFreqs(const vector<string>& terms) {
//     map<string, int> term_freqs;
//     for (const string& term : terms) {
//         ++term_freqs[term];
//     }
//     return term_freqs;
// }

// map<int, int> ComputeTermFreqs(const vector<int>& terms) {
//     map<int, int> term_freqs;
//     for (int term : terms) {
//         ++term_freqs[term];
//     }
//     return term_freqs;
// }

template <typename Term>  // шаблонный параметр-тип с названием Term
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;

    for (const Term& term : terms) {
        ++term_freqs[term];
    }

    return term_freqs;
}

int main() {
    const vector<int> leg_counts = {4, 2, 4, 4};
    const auto legs_stat = ComputeTermFreqs(leg_counts);
    cout << "Двуногих "s << legs_stat.at(2) << ", "s
         << "четвероногих "s << legs_stat.at(4) << endl;
    // Двуногих 1, четвероногих 3
}
