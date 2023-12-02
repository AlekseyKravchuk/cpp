#include <set>
#include <vector>

// template <typename T>
// std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
//     auto it = elements.upper_bound(border);
//     return (it != elements.end()) ? std::vector<T>(it, elements.end()) : std::vector<T>{};
// }

// менее элегантное решение
template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
    auto it = elements.begin();
    while (it != elements.end() && (*it) <= border) {
        ++it;
    }
    return std::vector<T>(it, elements.end());
}