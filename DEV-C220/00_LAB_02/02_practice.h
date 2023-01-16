#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

template <typename T>
class VectorWithUniques : public std::vector<T> {
   private:
    std::set<T> _uniques;

   public:
    VectorWithUniques() = default;
    VectorWithUniques(const std::initializer_list<T>& values, std::pair<T, T> range) {
        // std::copy_if(std::begin(values), std::end(values),
        //              std::inserter(_uniques, _uniques.begin()),
        //              [](auto val) {
        //                  return (val >= range.first && val <= range.second);
        //              });

        for (auto it = std::begin(values); it != std::end(values); ++it) {
            // если такой элемент уже добавлялся ранее, переходим к следующему элементу "std::initializer_list<T>"
            if (_uniques.count(*it)) {
                continue;
            }

            if (*it >= range.first && *it <= range.second) {
                _uniques.insert(*it);
                this->push_back(*it);
            }
        }
    }
};