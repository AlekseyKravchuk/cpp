#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <list>
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

    // добавляем в вектор только те значения, которых ещё нет в векторе (уникальные значения)
    VectorWithUniques& Append(const std::initializer_list<T>& values) {
        this->reserve(this->size() + std::size(values));

        for (const auto& val : values) {
            if (isInRange(val) &&
                std::find(this->begin(), this->end(), val) == this->end()) {
                this->push_back(val);
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

            auto itToDelete = std::find(begin(), end(), val);

            // если значение найдено в веторе, удаляем его
            if (itToDelete != end()) {
                this->erase(itToDelete);
            }
        }

        return *this;
    }

    void ChangeRange(const std::pair<T, T> new_range) {
        SetNewRange(new_range);

        for (auto it = begin(); it != end();) {
            // если текущее значение не попадает в новый диапазон, удаляем его из вектора
            if (!isInRange(*it)) {
                // после этого итератор "it" станет невалидным, поэтому
                // необходимо сохранить итератор, возвращаемый методом "std::vector::erase"
                it = this->erase(it);
            } else {
                ++it;
            }
        }
    }

    template <typename Pred>
    void Sort(Pred predicate) {
        std::sort(begin(), end(), predicate);
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