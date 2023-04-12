#include <chrono>
#include <cstdint>

using days = std::chrono::duration<uint64_t, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

int main() {
    
    return 0;
}
