#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> v{8}; // initialize the vector by providing an initializer list with one element
    v.reserve(2);
    size_t i = 1;

    /*=========== 1 ===========*/
    // if (i < v.size()) {
    //     cout << v[i] << endl;
    // } else {
    //     cout << "Attempt to go out of the vector bounds." << endl;
    // }

    /*=========== 2 ===========*/
    // try {
    //     cout << v.at(i) << endl;
    // } catch (const std::exception& e) {
    //     std::cerr << e.what() << '\n';
    // }

    /*=========== 3 ===========*/
    // Просим компилятор проверять выход за границы вектора.
    // Включаем отладочные макроопределения препроцессора (DEBUGGING FLAGS): -D_GLIBCXX_DEBUG и -D_GLIBCXX_DEBUG_PEDANTIC
    cout << v[i] << endl;

    // Но как же найти конкретную строку в программе, в которой локализована проблема?
    // Достаточно просто запустить программу в режиме DEBUG.

    return 0;
}
