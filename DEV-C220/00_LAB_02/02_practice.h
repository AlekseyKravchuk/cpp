#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

template <typename T>
class VectorWithUniques : protected std::vector<T> {
   private:
    std::pair<T, T> _range{};

   public:
    VectorWithUniques() = default;
    VectorWithUniques(const VectorWithUniques&) = default;
    VectorWithUniques& operator=(const VectorWithUniques&) = default;

    VectorWithUniques(VectorWithUniques&&) = default;
    VectorWithUniques& operator=(VectorWithUniques&&) = default;

    VectorWithUniques(const std::initializer_list<T>& values, const std::pair<T, T>& range) : _range(range) {
        this->reserve(std::size(values));

        for (auto it = std::begin(values); it != std::end(values); ++it) {
            if (*it >= range.first &&
                *it <= range.second &&
                std::find(this->begin(), this->end(), *it) == this->end()) {
                this->push_back(*it);
            }
        }
    }

    void SetNewRange(std::pair<T, T> new_range) {
        _range = new_range;
    }

    bool isInRange(T val) {
        return (val >= _range.first && val <= _range.second);
    }

    VectorWithUniques& Append(const std::initializer_list<T>& values) {
        this->reserve(this->size() + std::size(values));

        for (const auto& val : values) {
            if (val >= _range.first &&
                val <= _range.second &&
                std::find(this->begin(), this->end(), val) == this->end()) {
                this->push_back(val);
            }
        }

        return *this;
    }

    VectorWithUniques& Remove(const std::initializer_list<T>& values) {
        for (const auto& val : values) {
            if (val < _range.first || val > _range.second) {
                continue;
            }

            auto itToDelete = std::find(begin(), end(), val);
            if (itToDelete != end()) {
                this->erase(itToDelete);
            }
        }

        return *this;
    }

    void ChangeRange(const std::pair<T, T> new_range) {
        SetNewRange(new_range);

        for (auto it = begin(); it != end();) {
            if (!isInRange(*it)) {
                it = this->erase(it);
            } else {
                ++it;
            }
        }
    }

    template <typename Pred>
    void Sort(Pred predicate) {
        
    }

    size_t size() const {
        return this->std::vector<T>::size();
    }

    auto begin() {
        return this->std::vector<T>::begin();
    }

    auto begin() const {
        return std::vector<T>::cbegin();
    }

    auto end() {
        return std::vector<T>::end();
    }

    auto end() const {
        return std::vector<T>::cend();
    }
};