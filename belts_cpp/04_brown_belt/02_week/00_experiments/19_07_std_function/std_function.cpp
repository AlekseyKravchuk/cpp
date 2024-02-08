#include <functional>
#include <iomanip>
#include <iostream>

using namespace std;

// C-style function
bool my_greater(int x, int y) {
    return x > y;
}

struct Equal {
    Equal() {
        cout << "Equal::Equal() called " << endl;
    }

    Equal(const Equal& other) {
        cout << "Equal copy constructor called " << endl;
    }

    Equal(Equal&& other) {
        cout << "Equal MOVE constructor called " << endl;
    }

    bool operator()(int x, int y) {
        return x == y;
    }
};

struct S {
    bool not_equal(int x, int y) const {
        return x != y;
    }
};

int main() {
    std::function<bool(int, int)> f = [](int x, int y) {
        return x < y;
    };

    cout << boolalpha << f(10, 100) << endl;

    // ================================================================================

    // f = my_greater;  // вместо указателя на функцию можно просто использовать ее имя
    f = &my_greater;
    cout << boolalpha << f(10, 100) << endl;

    // ================================================================================
    f = std::move(Equal());
    cout << boolalpha << f(10, 100) << endl;
    // ================================================================================
    std::function<bool(const S&, int, int)> f2 = &S::not_equal;
    S s_obj;
    cout << boolalpha << f2(s_obj, 10, 100) << endl;

    return 0;
}
