#include <algorithm>
#include <iostream>
#include <numeric>  // std::iota
#include <string>
#include <vector>

using namespace std;

int main() {

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
                    }
                }
            }
        }
    }

    return 0;
}
