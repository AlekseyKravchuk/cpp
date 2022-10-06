#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto itToBeDeleted = find(v.begin(), v.end(), 5);
    auto itNewAfterDeletion = v.erase(itToBeDeleted);
    cout << "*itNewAfterDeletion = " << *itNewAfterDeletion << endl;
    
    for(auto elm : v) {
        cout << elm << " "s;
    }
    cout << endl;

    return 0;
}
