#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    std::map<int, std::string> m = {{2, "Foo"},
                                    {1, "Moo"},
                                    {5, "Goo"},
                                    {3, "Boo"}};

    std::cout << "m.size() = " << m.size() << std::endl; // 1

    // extract and check if node handle is not empty before use
    if (auto nh = m.extract(5); !nh.empty()) {
        std::cout << nh.key()
                  << "=>"
                  << nh.mapped() << std::endl; // 1=>Foo
    }                                          // node and the contained element are destroyed here

    // map is empty now
    std::cout << "m.size() = " << m.size() << std::endl; // 0
    return 0;
}
