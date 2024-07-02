#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// My version of function
template <template <typename> class containerType1, template <typename> class containerType2, typename containerElemType, typename elemTypeTerm>
vector<double> ComputeTfIdfs(const containerType1<containerType2<containerElemType>>& documents, const elemTypeTerm& word) {
    map<int, double> term_freqs;
    vector<double> tf_idfs;
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
        tf_idfs.push_back(term_freqs[docID] * IDF);
    }

    return tf_idfs;
}

// Yandex version of function
/* template <typename Documents, typename Term>
vector<double> ComputeTfIdfs(const Documents& documents, const Term& term) {
    vector<double> tf_idfs;

    int document_freq = 0;
    for (const auto& document : documents) {
        const int freq =  
        if (freq > 0) {
            ++document_freq;
        }
        tf_idfs.push_back(static_cast<double>(freq) / document.size());
    }

    const double idf = log(static_cast<double>(documents.size()) / document_freq);
    for (double& tf_idf : tf_idfs) {
        tf_idf *= idf;
    }

    return tf_idfs;
} */

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