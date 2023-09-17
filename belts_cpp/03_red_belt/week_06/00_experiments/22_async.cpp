#include <future> // std::async
#include <iostream>

int factorial(int N) {
    int res = 1;

    for (int i = N; i > 1; --i) {
        res *= i;
    }

    // std::cout << "res = " << res << std::endl;
    return res;
}

int main() {
    int x;
    
    std::future<int> fu = std::async(std::launch::async, factorial, 4);
    x = fu.get(); // в этом месте мы дожидаемся завершения дочернего потока

    std::cout << "x = " << x << std::endl;

    return 0;
}
