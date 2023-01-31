#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>


using namespace std::literals;

template <typename Container>
auto sum(const Container& c) {
    // std::remove_const_t<std::remove_reference_t<decltype(*std::begin(c))>> res_sum{}; // OK

    // улучшаем красоту кода посредством "using"
    // using ResType = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(c))>>;

    // существует средство, которое снимает ВСЕ квалификаторы: "std::decay_t"
    using ResType = std::decay_t<decltype(*std::begin(c))>;
    ResType res_sum{};

    for (auto elm : c) {
        res_sum += elm;
    }

    return res_sum;
}

int main() {
    std::vector<int> v{5, 15, 25};
    std::cout << sum(v) << std::endl;
    
    return 0;
}
