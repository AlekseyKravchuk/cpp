#include <iostream>
#include <numeric>  // std::accumulate
#include <vector> 

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int acc = 0;
    std::accumulate(v.begin(), v.end(), acc);

    std::cout << acc << std::endl;
    
    return 0;
}
