#include <iostream>
#include <vector>

using std::endl;
using std::cout;
using std::vector;

int main() {
    vector<int> v{3, 2, 9, 2, 8, 10, 11};
    int n = v.size();
    int x = 17;

    for(auto i = 0; i < n; i++ ) {
        cout << "\n" << v[i] << ":" << endl << "\t";
        for(auto j = i+1; j < n; j++) {
            cout << v[j] << "  " ;
        }
    }
}
