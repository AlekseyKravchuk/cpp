#include <cassert>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

// ========================= Базовая версия функции, предложенная в задании =========================
// ========================= без отладочных выводов в консоль =========================
// template <typename RandomIt>
// void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
//     // Тип переупорядочиваемых объектов можно получить с помощью выражения "typename RandomIt::value_type"
//     vector<typename RandomIt::value_type> pool(first, last);

//     size_t cur_pos = 0;

//     while (!pool.empty()) {
//         // *(first++) = pool[cur_pos];

//         pool.erase(pool.begin() + cur_pos);

//         if (pool.empty()) {
//             break;
//         }

//         cur_pos = (cur_pos + step_size - 1) % pool.size();
//     }
// }

// // ========================= Базовая версия функции, предложенная в задании =========================
// template <typename RandomIt>
// void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
//     // Тип переупорядочиваемых объектов можно получить с помощью выражения "typename RandomIt::value_type"
//     vector<typename RandomIt::value_type> pool(first, last);

//     size_t cur_pos = 0;

//     int iterationNum = 1;
//     while (!pool.empty()) {
//         std::cout << "\n\n*********************** Итерация №"s << iterationNum << " ***********************"s << std::endl;
//         std::cout << "================== В самом начале цикла =================="s << std::endl;
//         std::cout << "*(first) = " << *(first) << std::endl;
//         *(first++) = pool[cur_pos];

//         std::cout << "================== После *(first++) = pool[cur_pos]; =================="s << std::endl;
//         std::cout << "*(first) = " << *(first) << std::endl;

//         std::cout << "================== Перед удалением ==================" << std::endl;
//         std::cout << "Element \""s << *(pool.begin() + cur_pos) << "\" will be deleted."s << std::endl;
//         std::cout << "cur_pos = " << cur_pos << ", *(pool.begin() + cur_pos) = " << *(pool.begin() + cur_pos) << std::endl;
//         std::cout << "======================================================" << std::endl;

//         pool.erase(pool.begin() + cur_pos);

//         if (pool.empty()) {
//             break;
//         }

//         std::cout << "\n================== Перед пересчетом позиции (после удаления) ==================" << std::endl;
//         size_t poolSize = pool.size();
//         std::cout << "poolSize = " << poolSize << std::endl;
//         PrintRange(pool.begin(), pool.end());

//         cur_pos = (cur_pos + step_size - 1) % poolSize;

//         std::cout << "\n================== После пересчета позиции ==================" << std::endl;
//         std::cout << "cur_pos = " << cur_pos << ", *(pool.begin() + cur_pos) = " << *(pool.begin() + cur_pos) << std::endl;
//         std::cout << "vector: ";
//         PrintRange(pool.begin(), pool.end());

//         ++iterationNum;
//     }
// }

// ========================= Версия функции, предложенная в треде Slack'а =========================
// template <typename RandomIt>
// void MakeJosephusPermutationSlack(RandomIt first, RandomIt last, uint32_t step_size) {
//     vector<typename RandomIt::value_type> pool;
//     std::move(first, last, back_inserter(pool));
//     size_t cur_pos = 0;

//     while (first != last) {
//         *(first++) = std::move(*(pool.begin() + cur_pos));

//         std::cout << "Element \""s << *(pool.begin() + cur_pos) << "\" will be deleted."s
//                   << "\t";
//         pool.erase(pool.begin() + cur_pos);

//         if (first == last) {
//             break;
//         }

//         cur_pos = (cur_pos + step_size - 1) % pool.size();

//         // ------------- Отладочная информация -------------
//         std::cout << "cur_pos = " << cur_pos << ", pool.begin() + cur_pos = " << *(pool.begin() + cur_pos);
//         std::cout << "\tvector: ";
//         PrintRange(pool.begin(), pool.end());
//     }
// }

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {

    // Тип переупорядочиваемых объектов можно получить с помощью выражения "typename RandomIt::value_type"
    std::list<typename RandomIt::value_type> pool(make_move_iterator(first), make_move_iterator(last));

    size_t cur_pos = 0;

    while (!pool.empty()) {
        auto it = pool.begin();

        advance(it, cur_pos);
        *(first++) = move(*it);

        it = pool.erase(it);

        if (pool.empty()) {
            break;
        }

        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    uint32_t step_size = 3;

    // MakeJosephusPermutationSlack(v.begin(), v.end(), step_size);
    MakeJosephusPermutation(v.begin(), v.end(), step_size);
    PrintRange(v.begin(), v.end());
}