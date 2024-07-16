#pragma once

#include "paginator.h"
#include "utilities.h"

#include <vector>
#include <algorithm>
#include <future>

/*
Сортировка ОБЫЧНО работает за O(Nlog(N)), и это достаточно медленно для больших объемов чисел.
Для натуральных чисел, когда разница между минимальными и максимальным числамиз в диапазоне небольшая,
можно огранизовать более эффектвные способы сортировки, работающие за O(N).
Самая тривиальная сортировка подобного рода - это "сортировка подсчетом" (counting sort)
*/
template <typename Iterator>
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

// ==========================================================================================================
//template <typename Iterator>
//void CountingSort_SingleThread(Iterator first,
//                               Iterator second,
//                               std::vector<size_t>::iterator it_counter_begin,
//                               typename std::iterator_traits<Iterator>::value_type min_val,
//                               typename std::iterator_traits<Iterator>::value_type max_val) {
//    if (first == second) {
//        return;
//    }
//
//    // заполняем counter
//    for (auto it = first; it != second; ++it) {
//        *(it_counter_begin + (*it - min_val)) += 1;  // эквивалент "++counter[*it - min_val];"
//    }
//
//    // восстанавливаем кусок вектора (в отсортированном виде), на который указывают итераторы first и second
//    size_t n_repetitions;
//    for (auto i = min_val; i <= max_val; ++i) {
//        n_repetitions = *(it_counter_begin + (i - min_val));  // эквивалент "n_repetitions = counter[i - min_val];"
//
//        for (auto j = 0U; j < n_repetitions; ++j) {
//            *(first++) = i;
//        }
//    }
//}
// ==========================================================================================================

template <typename Iterator>
void CountingSort_MultiThread(Iterator first, Iterator second) {
    if (first == second) {
        return;
    }

    auto [min_it, max_it] = std::minmax_element(first, second);  // O(N)
    auto min_val = *min_it;
    auto max_val = *max_it;

    // 1) определяем размер каждой страницы (chunk'а) исходя из имеющихся ресурсов
    int num_threads = std::min(8, static_cast<int>(std::thread::hardware_concurrency()));
    size_t page_size = std::distance(first, second) / num_threads;

    // 2) разбиваем исходный вектор на страницы
    auto pages = Paginate(first, second, page_size);

    // 3) Заводим общий вектор-counter, который, в свою очередь, также разбиваем на страницы
    size_t each_counter_size = max_val - min_val + 1;
    std::vector<unsigned int> counter(each_counter_size * pages.size(), 0);
    auto counters = Paginate(counter, each_counter_size);

    // 4) Создаем вектор фьюч и для каждой страницы асинхронно вызывыем CountingSort_SingleThread
    std::vector<std::future<void>> futures;
    for (size_t k = 0; k < pages.size(); ++k) {
        // TODO: реализовать запуск шаблонной функции CountingSort_SingleThread в std::async
//        futures.push_back(std::async(std::launch::async, [&]() mutable {
//            CountingSort_SingleThread(pages[k].begin(),
//                                      pages[k].end(),
//                                      counters[k].begin(),
//                                      min_val,
//                                      max_val);
//        }));

        // ====================================
        futures.push_back(std::async(std::launch::async,
                                     CountingSort_SingleThread,
                                     pages[k].begin(),
                                     pages[k].end(),
                                     counters[k].begin(),
                                     min_val,
                                     max_val));
    }

    // 5) По завершении асинхронных вызовов CountingSort_SingleThread имеем заполненные counter'ы.
    for (auto& future : futures) {
        future.get();
    }
//    futures.clear();

    // 6) К первому counter'y добавляем оставшиеся из counters.size()
    auto it_ctr_begin = counters[0].begin();
    auto it_ctr_end = counters[0].end();

    for (size_t j = 1; j < counters.size(); ++j) {

        auto it1 = it_ctr_begin;
        auto it2 = counters[j].begin();

        while (it1 != it_ctr_end) {
            *(it1++) += *(it2++);
        }
    }

    // 7) Восстанавливаем вектор в отсортированном виде
    size_t n_repetitions;
    for (auto i = min_val; i <= max_val; ++i) {
        n_repetitions = *(it_ctr_begin + (i - min_val));  // эквивалент "n_repetitions = counter[i - min_val];"

        for (auto j = 0U; j < n_repetitions; ++j) {
            *(first++) = i;
        }
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

    // 1) определяем размер каждой страницы (chunk'а) исходя из имеющихся ресурсов
    int num_threads = std::min(8, static_cast<int>(std::thread::hardware_concurrency()));
    size_t page_size = std::distance(first, second) / num_threads;

    // 2) разбиваем исходный вектор на страницы
    auto pages = Paginate(first, second, page_size);  // pages: вектор пар итераторов на начало и конец страницы

    // 3) Создаем вектор фьюч и для каждой страницы асинхронно вызывыем CountingSort
    std::vector<std::future<void>> futures;
    for (size_t k = 0; k < pages.size(); ++k) {
        futures.push_back(std::async(CountingSort<Iterator>,
                                     pages[k].begin(),
                                     pages[k].end()));
    }

    // 4) По завершении асинхронных вызовов CountingSort имеем отсортированные chunk'и (страницы).
    for (auto& future : futures) {
        future.get();
    }
//    futures.clear();

    // 5) Резервируем место для результирующего вектора
    std::vector<ValueType> result;
    result.reserve(std::distance(first, second));

    // 6) Копируем первую страницу в результирующий вектор
    result.insert(result.end(), pages[0].begin(), pages[0].end());

    // 7) Итеративное слияние остальных страниц
    for (size_t i = 1; i < pages.size(); ++i) {
        std::vector<ValueType> temp;

        temp.reserve(result.size() + pages[i].size());
        std::merge(result.begin(), result.end(),
                   pages[i].begin(), pages[i].end(),
                   std::back_inserter(temp));
        result.swap(temp);
    }

    // 8) Копируем результирующий вектор обратно в исходный вектор
    std::copy(result.begin(), result.end(), first);
}


