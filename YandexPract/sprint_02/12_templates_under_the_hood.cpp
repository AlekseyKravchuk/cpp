#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

using namespace std;

template <typename T>
vector<double> ComputeTfIdfs(const vector<vector<T>>& documents, const T& word2search) {
    static int docCount = 0;

    for (const auto& document : documents) {
        map<T, double> word_IDF;  // 
        map<T, int> 
        for (const auto& currentWord : document) {
        }
        // после того, как обработали текущий документ инкрементим docCount
        ++docCount;
    }
}

int main() {
    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    cout << endl;
    return 0;
}