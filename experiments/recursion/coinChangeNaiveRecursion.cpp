/*
 * #################### Problem statement: ####################
 * You are given infinite supply of coins of different denominations and the total amount N.
 * Find the number of ways to make change for value N using the coins.
 */
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

using std::vector;
using std::cout;
using std::endl;

/*
 * ========== Explanation ==========
 * At each recursive step we have only 2 choices: include or exclude current denomination from a set.
 * If we include current denomination, than index idx stay unchanged, otherwise it will be incremented
 * to show that the previous coin denomination is excluded from consideration (from a coin denomination set).
 */
int coinChangeNaiveRecursive(const vector<int>& denominations, int idx, int N) {
    // ========== base cases ==========
    if(N == 0) { // after successive subtraction coin denominations from N we get 0, so this is a way to make change
        return 1;
    }

    // corresponding recursive subtree is not a way to make a change (make given amount)
    // or
    // set of denominations became empty
    if(N < 0 or idx == denominations.size()) {
        return 0;
    }

    return coinChangeNaiveRecursive(denominations, idx, N - denominations[idx])
        + coinChangeNaiveRecursive(denominations, idx+1, N);
}


int main() {
    int N{0};
    vector<int> denominations;
    int x;

    cout << "Type in the total amount(N) that would be changed for: ";
    std::cin >> N;
    cout << "Type in coin denominations to be used for change the total amount separated by spaces: ";

    while (std::cin >> x) {
        denominations.push_back(x);
    }
    std::sort(denominations.begin(), denominations.end());

    // print all the elements of denominations vector in ascending order
    cout << "Ordered denominations: ";
    // std::for_each(denominations.begin(), denominations.end(), [](int i)->void {cout << i << " ";});
    for(const auto coin: denominations) {
        cout << coin << " ";
    }
    cout << endl;

    int n = coinChangeNaiveRecursive(denominations, 0, N);
    cout << "The number of ways = " << n << endl;

    return 0;
}
