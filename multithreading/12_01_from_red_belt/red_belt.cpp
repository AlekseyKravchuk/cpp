#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int SumTwoVectors(const vector<int>& one, const vector<int>& two) {
    return accumulate(begin(one), end(one), 0) + accumulate(begin(two), end(two), 0);;
}

int main() {
    cout << SumTwoVectors( {1, 1, 1, 1, 1},
                           {3, 3, 3, 3, 3} );
    
    return 0;
}
