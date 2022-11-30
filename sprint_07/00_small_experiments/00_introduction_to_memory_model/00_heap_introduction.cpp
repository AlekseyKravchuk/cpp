#include <iostream>
#include <sstream>  // std::istringstream::istringstream
#include <vector>

using namespace std;

void fill(istream& is, vector<int>& ints) {
    int x;
    while (is >> x) {
        ints.push_back(x);
    }
}

int main() {
    vector<int> ints1, ints2;

    istringstream iss1("1 2 3 4 5"s);
    fill(iss1, ints1);

    istringstream iss2("6 7 8"s);
    fill(iss2, ints2);

    return 0;
}
