#include "crtp_interface.hpp"
#include "normal_interface.hpp"
#include <chrono>

template <typename T, typename S>
void Benchmark(T& object, S function, const std::string& name) {
    // Get starting timepoint
    auto start = std::chrono::high_resolution_clock::now();

    // Execute the function
    function(&object);

    // Get ending timepoint
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate time and output result
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << name << ": " << duration.count() << std::endl;
}

int main() {
    NormalImplemented object_normal;
    Benchmark(object_normal, RunNormal, "Normal Interface");

    CRTPImplemented object_crtp;
    Benchmark(object_crtp, RunCRTP<CRTPImplemented>, "CRTP Interface");

    return 0;
}