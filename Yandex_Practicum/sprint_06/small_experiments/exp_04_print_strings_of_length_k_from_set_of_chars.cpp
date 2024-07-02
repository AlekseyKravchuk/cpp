// C++ program to print all possible strings of length k using a set of characters
#include <iostream>
#include <numeric>  // std::iota
#include <string>
#include <vector>

using namespace std;

// The main recursive method to print all possible strings of length k
void printAllKLengthRec(vector<char> alphabet, string prefix, int n, int k) {
    // Base case: k is 0, print prefix
    if (k == 0) {
        cout << (prefix) << endl;
        return;
    }

    // One by one add all characters from set and recursively call for k equals to k-1
    for (int i = 0; i < n; i++) {
        string newPrefix;

        // Next character of input added
        newPrefix = prefix + alphabet[i];

        // k is decreased, because we have added a new character
        printAllKLengthRec(alphabet, newPrefix, n, k - 1);
    }
}

void printAllKLength(vector<char> alphabet, int k, int n) {
    printAllKLengthRec(alphabet, "", n, k);
}

// Driver Code
int main() {
    cout << "First Test" << endl;

    // std::vector<char> alphabet(26);
    // std::iota(alphabet.begin(), alphabet.end(), 'A');

    vector<char> alphabet = {'a', 'b', 'c', 'd'};
    int wordLen = 5;
    printAllKLength(alphabet, wordLen, alphabet.size());
}
