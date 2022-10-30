#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    int predictedAmount = static_cast<int64_t>(v.size()) * (i + 1) / (n + 1);
    int numOfLessOrEqual;

    if (predictedAmount > std::log2(v.size())) {
        std::cout << "Using upper_bound" << std::endl;
        numOfLessOrEqual = std::distance(v.begin(), std::upper_bound(v.begin(), v.end(), i));
    } else {
        std::cout << "Using find_if" << std::endl;
        numOfLessOrEqual = std::distance(v.begin(),
                                         std::find_if(v.begin(), v.end(), [i](int val) {
                                             return val > i;
                                         }));
    }

    return numOfLessOrEqual;
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;
    // static const int NUMBERS = 10'000;
    // static const int MAX = 5'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}