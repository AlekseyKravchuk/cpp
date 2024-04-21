#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>  // std::accumulate
#include <vector>

using namespace std;

int SumTwoVectors(const vector<int>& one,
                  const vector<int>& two) {
    return std::accumulate(one.begin(), one.end(), 0) +
           std::accumulate(two.begin(), two.end(), 0);
}

int SumTwoVectorsMultithreaded(const vector<int>& one,
                               const vector<int>& two) {
    std::future<int> fu = std::async(std::launch::async,
                                     [&one]() {
                                         return std::accumulate(one.begin(), one.end(), 0);
                                     });

    int result = std::accumulate(two.begin(), two.end(), 0);

    return result + fu.get();
}

int main() {
    // cout << SumTwoVectors({1, 1, 1, 1, 1},
    //                       {5, 5, 5, 5, 5})
    //      << endl;

    cout << SumTwoVectorsMultithreaded({1, 1, 1, 1, 1},
                                       {5, 5, 5, 5, 5})
         << endl;
    return 0;
}
