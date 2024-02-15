#include <functional>  // std::bind
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
// using namespace std::placeholders;

void print(const vector<string>& vstr, int value) {
    const size_t MAX = std::min(static_cast<int>(vstr.size()), value);
    for (size_t i = 0; i < MAX; ++i) {
        cout << vstr[i] << endl;
    }
}

int main() {
    const vector<string> v = {"one", "two", "three", "four", "five", "six", "seven"};

    print(v, 3);

    cout << "==============================================" << endl;

    // здесь происходит полное копирование вектора при вызове std::bind
    auto print_elegant_by_copying = std::bind(print, v, std::placeholders::_1);
    print_elegant_by_copying(1);

    cout << "==============================================" << endl;
    // избавляемся от копирования, передаем вектор по константной ссылке (use reference wrapper std::cref)
    auto print_elegant = std::bind(print, std::cref(v), std::placeholders::_1);
    print_elegant(5);


    return 0;
}
