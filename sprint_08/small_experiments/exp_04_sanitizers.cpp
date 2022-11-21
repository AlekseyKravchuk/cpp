#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const string& MakeString() {
    const vector<string> strings = {
        "C++"s,
        "Python"s};

    // возвращаем ссылку на строчку, которая будет невалидной
    return strings[0];
}

int main() {
    const auto& ref = MakeString();
    cout << ref << endl;
    
    return 0;
}
