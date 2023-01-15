#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // 5 1 2 3 4 5
    int ratings_size;
    cin >> ratings_size;

    // создали вектор размера ratings_size из нулей
    vector<int> ratings(ratings_size, 0);

    // считали каждый элемент с помощью ссылки
    for (int& rating : ratings) {
        cin >> rating;
    }

    for (auto& elm : ratings) {
        cout << elm << " ";
    }
    cout << endl;

    return 0;
}
