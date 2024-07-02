#include <algorithm>
#include <iostream>
#include <numeric>  // std::iota
#include <string>
#include <vector>

using namespace std;

template <typename F>
string BruteForce(F check) {
    for (char ch1 = 'A'; ch1 <= 'Z'; ch1++) {
        for (char ch2 = 'A'; ch2 <= 'Z'; ch2++) {
            for (char ch3 = 'A'; ch3 <= 'Z'; ch3++) {
                for (char ch4 = 'A'; ch4 <= 'Z'; ch4++) {
                    for (char ch5 = 'A'; ch5 <= 'Z'; ch5++) {
                        std::string str;
                        str += ch1;
                        str += ch2;
                        str += ch3;
                        str += ch4;
                        str += ch5;
                        if (check(str)) {
                            return str;
                        }
                    }
                }
            }
        }
    }

    return ""s;
}

int main() {
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}