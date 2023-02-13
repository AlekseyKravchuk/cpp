#include <type_traits>  // std::is_same_v
#include <vector>

template <typename FirstType, typename SecondType>
SecondType summation(FirstType& x, SecondType y) {
    SecondType result_sum{};

    if constexpr (std::is_same_v<decltype(x), std::vector<SecondType>&>) {
        for (auto& elm : x) {
            elm += y;
            result_sum += elm;
        }
    } else {
        static_assert(std::is_same_v<FirstType, SecondType>);
        result_sum = x + y;
    }

    return result_sum;
}