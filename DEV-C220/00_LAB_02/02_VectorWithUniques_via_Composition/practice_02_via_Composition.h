#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <list>
#include <set>
#include <utility>
#include <vector>

template <typename T>
class VectorWithUniques {
   private:
    std::pair<T, T> _range{};
    std::vector<T> _v{};

   public:
    VectorWithUniques() = default;
    VectorWithUniques(const VectorWithUniques&) = default;
    VectorWithUniques& operator=(const VectorWithUniques&) = default;

    VectorWithUniques(VectorWithUniques&&) = default;
    VectorWithUniques& operator=(VectorWithUniques&&) = default;

    VectorWithUniques(const std::initializer_list<T>& values, const std::pair<T, T>& range) : _range(range) {
        _v.reserve(std::size(values));

        for (auto it = std::begin(values); it != std::end(values); ++it) {
            if (*it >= range.first &&
                *it <= range.second &&
                std::find(_v.begin(), _v.end(), *it) == _v.end()) {
                _v.push_back(*it);
            }
        }
    }

    void SetNewRange(std::pair<T, T> new_range) {
        _range = new_range;
    }

    bool isInRange(T val) {
        return (val >= _range.first && val <= _range.second);
    }

    // добавляем в вектор только те значения, которых ещё нет в векторе (уникальные значения)
    VectorWithUniques& Append(const std::initializer_list<T>& values) {
        _v.reserve(this->size() + std::size(values));

        for (const auto& val : values) {
            if (isInRange(val) &&
                std::find(_v.begin(), _v.end(), val) == _v.end()) {
                _v.push_back(val);
            }
        }

        return *this;
    }

    VectorWithUniques& Remove(const std::initializer_list<T>& values) {
        for (const auto& val : values) {
            // если значение не попадает в допустимый диапазон, просто игнорируем его
            if (!isInRange(val)) {
                continue;
            }

            auto itToDelete = std::find(_v.begin(), _v.end(), val);

            // если значение найдено в веторе, удаляем его
            if (itToDelete != _v.end()) {
                _v.erase(itToDelete);
            }
        }

        return *this;
    }

    void ChangeRange(const std::pair<T, T> new_range) {
        SetNewRange(new_range);

        for (auto it = _v.begin(); it != _v.end();) {
            // если текущее значение не попадает в новый диапазон, удаляем его из вектора
            if (!isInRange(*it)) {
                // после этого итератор "it" станет невалидным, поэтому
                // необходимо сохранить итератор, возвращаемый методом "std::vector::erase"
                it = _v.erase(it);
            } else {
                ++it;
            }
        }
    }

    template <typename Pred>
    void Sort(Pred predicate) {
        std::sort(_v.begin(), _v.end(), predicate);
    }

    size_t size() const {
        return _v.size();
    }

    auto begin() {
        return _v.begin();
    }

    auto begin() const {
        return _v.cbegin();
    }

    auto end() {
        return _v.end();
    }

    auto end() const {
        return _v.cend();
    }
};