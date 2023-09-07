#include <map>
#include <set>
#include <vector>

template <typename T>
class PriorityCollection {
  public:
    using Id = size_t;    // тип, используемый для идентификаторов
    using Priority = int; // тип, используемый для приоритета

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        Id index = _storage.size();
        _storage.push_back(std::move(object));

        _pr_to_indices[0].insert(index);
        _id_to_pr[index] = 0;

        return index;
    }

    // Добавить все элементы диапазона [range_begin, range_end) с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; ++it) {
            *ids_begin = Add(move(*it));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return _id_to_pr.count(id) == 1;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _storage[id];
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        auto old_priority = _id_to_pr.at(id);

        auto& prior_list = _pr_to_indices[old_priority];
        prior_list.erase(id);

        if (prior_list.empty())
            _pr_to_indices.erase(old_priority);

        auto new_priority = ++_id_to_pr[id];

        _pr_to_indices[new_priority].insert(id);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto max_it = prev(_pr_to_indices.end());
        auto priority = max_it->first;

        auto& prior_set = max_it->second;

        assert(!prior_set.empty());

        auto max_id_iter = prev(prior_set.end());

        assert(max_id_iter != prior_set.end());

        Id id = *max_id_iter;
        return {_storage[id], priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto max_it = prev(_pr_to_indices.end());

        auto priority = max_it->first;
        auto& prior_set = max_it->second;

        assert(!prior_set.empty());

        auto max_id_iter = prev(prior_set.end());

        assert(max_id_iter != prior_set.end());

        Id id = *max_id_iter;

        prior_set.erase(max_id_iter);

        if (prior_set.empty())
            _pr_to_indices.erase(priority);

        _id_to_pr.erase(id);

        return {std::move(_storage[id]), priority};
    }

  private:
    // Приватные поля и методы

    std::map<Priority, std::set<Id>> _pr_to_indices; // словарь priority -> set_of_indexes
    std::map<Id, Priority> _id_to_pr;                 // словарь index -> priority
    std::vector<T> _storage;
};