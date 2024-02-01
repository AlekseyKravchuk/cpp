#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>

using namespace std;

int main() {
    int value = 4;
    auto increase = [value](int x) {
        // в данном случае константность нас защищает неявно, т.к. в анонимном классе, который генерируется
        // под лямбда-функцию, принимающую аргументы ПО ЗНАЧЕНИЮ, оператор вызова - operator() является КОНСТАНТНЫМ
        return value += x;  // error: assignment of read-only variable ‘value’
    };

cout << increase(5) << endl;
cout << increase(5) << endl;

    return 0;
}
