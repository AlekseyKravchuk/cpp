#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

template <typename T>
vector<double> ComputeTfIdfs(const vector<vector<T>>& documents, const T& word) {
    map<int, double> term_freqs;
    vector<double> res;
    int docID = 0;
    
    for (const auto& doc : documents) {
        for (const auto& term : doc) {
            if (term == word) {
                term_freqs[docID] += 1.0 / static_cast<double>(doc.size());
            }
        }
        ++docID;
    }
    int docCounter = docID;
    double IDF = log((docCounter) / static_cast<double>(term_freqs.size()));
    
    for (int docID = 0; docID < docCounter; ++docID) {
        res.push_back(term_freqs[docID] * IDF);
    }

    return res;
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