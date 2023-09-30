#include <iostream>

int main() {
    // auto& bucket = _buckets[static_cast<uint64_t>(key) % _buckets.size()];
    int key = -11;
    size_t sz = 20;

    uint64_t casted_key = static_cast<uint64_t>(key);

    std::cout << "key = " << key << "; casted_key = " << casted_key << std::endl;
    std::cout << key << " % 20 = " << key % sz << std::endl;
    std::cout << casted_key << " % 20 = " << casted_key % sz << std::endl;

    return 0;
}
