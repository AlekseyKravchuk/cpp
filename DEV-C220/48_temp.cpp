#include <chrono>
#include <iostream>
#include <set>
#include <sstream>  // std::stringstream
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals::chrono_literals;
using namespace std;

class A {
   public:
    explicit A(int) {}
    A(int, int) {}
};

void process(const A& a) { /*...*/
}

int main() {
    set<string> s = {"abc", "www", "qwerty"};
    using IT = set<string>::iterator;

    vector<string> v(move_iterator<IT>(s.begin()), move_iterator<IT>(s.end()));
    std::cout << "OK" << std::endl;
}