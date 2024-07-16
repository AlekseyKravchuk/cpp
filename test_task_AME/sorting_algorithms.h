#pragma once

#include "paginator.h"
#include "utilities.h"

#include <algorithm>
#include <future>
#include <vector>

template<typename Iterator>
void CountingSort(Iterator first, Iterator second) {
    if (first == second) {
        return;
    }

    auto [min_it, max_it] = std::minmax_element(first, second);  // O(N)
    auto min_val = *min_it;
    auto max_val = *max_it;
    std::vector<unsigned int> count(max_val - min_val + 1, 0);

    for (auto it = first; it != second; ++it) { // O(N)
        ++count[*it - min_val];
    }

    size_t n_repetitions;
    for (auto i = min_val; i <= max_val; ++i) {  // ~O(N), считаем, что вложенный цикл отрабатывает за O(1)
        n_repetitions = count[i - min_val];

        for (auto j = 0U; j < n_repetitions; ++j) {
            *(first++) = i;
        }
    }
    // суммарная сложность 3*O(N), но поскольку константы при оценке сложности отбрасываем, получаем O(N)
}

template<typename Iterator>
void CountingSort_MultiThread(Iterator first, Iterator second) {
    if (first == second) {
        return;
    }

    auto [min_it, max_it] = std::minmax_element(first, second);  // O(N)
    auto min_val = *min_it;
    auto max_val = *max_it;
    size_t range = max_val - min_val + 1;

    // Определяем количество потоков
    size_t num_threads = static_cast<size_t>(std::min(8, static_cast<int>(std::thread::hardware_concurrency())));
    size_t page_size = std::distance(first, second) / num_threads;

    // Разбиваем исходный вектор на страницы
    auto pages = Paginate(first, second, page_size);

    // Заводим общий вектор-counter, который, в свою очередь, также разбиваем на страницы
    size_t each_counter_size = max_val - min_val + 1;
    std::vector<unsigned int> counter(each_counter_size * pages.size(), 0);
    auto counters = Paginate(counter, each_counter_size);

    // Заводим вектор счетчиков для каждого потока
    std::vector<std::vector<unsigned int>> local_counts(num_threads, std::vector<unsigned int>(range, 0));

    // Запускаем потоки для заполнения локальных счетчиков
    std::vector<std::future<void>> futures;
    for (size_t i = 0; i < num_threads; ++i) {
        futures.push_back(std::async(std::launch::async,
                                     [=, &local_counts]() mutable {
                                         CountingSort_SingleThread(pages[i].begin(),
                                                                   pages[i].end(),
                                                                   local_counts[i].begin(),
                                                                   min_val,
                                                                   max_val);
                                     }));
    }

    // Ожидаем завершения всех потоков
    for (auto& future: futures) {
        future.get();
    }

    // Объединяем локальные счетчики в один глобальный счетчик
    std::vector<unsigned int> global_count(range, 0);
    for (const auto& local_count: local_counts) {
        for (size_t j = 0; j < range; ++j) {
            global_count[j] += local_count[j];
        }
    }

    // Восстанавливаем отсортированный диапазон
    auto output_it = first;
    for (size_t i = 0; i < range; ++i) {
        std::fill_n(output_it, global_count[i], min_val + i);
        output_it += global_count[i];
    }
}
// ==========================================================================================================

template <typename Iterator>
void CountingSort_MultiThread_Merge(Iterator first, Iterator second) {
    // Определение типа элементов, на которые указывает итератор
    using ValueType = typename std::iterator_traits<Iterator>::value_type;

    if (first == second) {
        return;
    }

    // Определяем количество потоков
    size_t num_threads = static_cast<size_t>(std::min(8, static_cast<int>(std::thread::hardware_concurrency())));
    size_t page_size = std::distance(first, second) / num_threads;

    // Разбиваем исходный вектор на страницы
    auto pages = Paginate(first, second, page_size); // pages: вектор пар итераторов на начало и конец страницы

    // Запускаем потоки, вызывая CountingSort для каждой страницы
    std::vector<std::future<void>> futures;
    for (size_t k = 0; k < pages.size(); ++k) {
        futures.push_back(std::async(std::launch::async,
                                     CountingSort<Iterator>,
                                     pages[k].begin(),
                                     pages[k].end()));
    }

    // Ожидаем завершения всех потоков. По завершении асинхронных вызовов имеем отсортированные страницы.
    for (auto& future : futures) {
        future.get();
    }

    // Резервируем место для результирующего вектора
    std::vector<ValueType> result;
    result.reserve(std::distance(first, second));

    // Копируем первую страницу в результирующий вектор
    result.insert(result.end(), pages[0].begin(), pages[0].end());

    // Сливаем (merge) страницы
    for (size_t i = 1; i < pages.size(); ++i) {
        std::vector<ValueType> temp;

        temp.reserve(result.size() + pages[i].size());
        std::merge(result.begin(),result.end(),
                   pages[i].begin(), pages[i].end(),
                   std::back_inserter(temp));
        result.swap(temp);
    }

    // 8) Копируем результирующий вектор обратно в исходный вектор
    std::copy(result.begin(), result.end(), first);
}
