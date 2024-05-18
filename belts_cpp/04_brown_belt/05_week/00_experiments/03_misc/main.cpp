#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "print.h"

using namespace std;

int main() {
    std::vector<string> stops = {
        "A",
        "B",
        "C",
        "C",
        "D",
        "B",
        "A"};
    size_t old_size = stops.size();
    stops.resize(old_size * 2 - 1);
    std::copy_backward(next(stops.rbegin()), stops.rend(),
                       stops.end());
    // std::copy(stops.rbegin(), stops.rend(),
    //           stops.end());

    // stops.insert(stops.end(),
    //              ++stops.rbegin(), stops.rend());

    cout << stops << endl;
}