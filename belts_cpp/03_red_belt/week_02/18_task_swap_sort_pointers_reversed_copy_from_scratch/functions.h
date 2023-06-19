#pragma once

#include <algorithm>

// обменивает местами значения, на которые указывают указатели first и second
template <typename T>
void Swap(T* first, T* second) {
    std::swap(*first, *second);
}

// сортирует указатели по значениям, на которые они указывают
template <typename T>
void SortPointers(std::vector<T*>& pointers) {
    std::sort(pointers.begin(), pointers.end(),
              [](T* lhs_ptr, T* rhs_ptr) {
                  return *lhs_ptr < *rhs_ptr;
              });
}

// копирует в обратном порядке count элементов, начиная с адреса в указателе src, в область памяти, начинающуюся по адресу dst.
// При этом:
// 1. каждый объект из диапазона [src; src + count) должен быть скопирован не более одного раза;
// 2. диапазоны [src; src + count) и [dst; dst + count) могут пересекаться
// 3. элементы в части диапазона [src; src + count), которая не пересекается с [dst; dst + count), должны остаться неизменными.
template <typename T>
void ReversedCopy(T* src, size_t count, T* dst) {
    T* src_end = src + count;
    T* dst_end = dst + count;
    T* max_of_starts = std::max(src, dst);
    T* min_of_ends = std::min(src_end, dst_end);

    // Если дипазоны НЕ перекрываются, то есть если max(L1, L2) > min(R1, R2), вызываем стандартный алгоритм std::copy_reverse
    if (max_of_starts > min_of_ends) {
        std::reverse_copy(src, src + count, dst);
        return;
    }

    // ====================== Если дипазоны ПЕРЕКРЫВАЮТСЯ ======================
    // УСЛОВИЕ ПЕРЕКРЫТИЯ ДИАПАЗОНОВ:
    // если дипазоны перекрываются, то бОльшее их начало будет меньше, чем меньший из их концов: max(L1, L2) < min(R1, R2)

    T* ovelapped_begin = max_of_starts;
    T* ovelapped_end = min_of_ends;
    std::reverse(ovelapped_begin, ovelapped_end);

    // после того, как привели к требуемому виду перекрывающуюся часть (overlapping part)
    // требуется установить порядок следования диапазонов [src, src_end) и [dst, dst_end),
    // после чего выполнить необходимое копирование оставшейся необработанной части
    if (dst > src) {
        std::reverse_copy(src, dst, src_end);
    } else {
        std::reverse_copy(dst_end, src_end, dst);
    }
}

// // =================== Простое и элегантное решение ===================
// template <typename T>
// void ReversedCopy(T* source, size_t count, T* destination) {
//     std::vector<T> v(source, source + count);

//     // std::move( InputIt first, InputIt last, OutputIt d_first ) defined in header <algorithm>
//     // it moves the elements in the range [first, last), to another range beginning at "d_first",
//     // starting from "first" and proceeding to "last - 1".
//     std::move(v.rbegin(), v.rend(), destination);
// }
