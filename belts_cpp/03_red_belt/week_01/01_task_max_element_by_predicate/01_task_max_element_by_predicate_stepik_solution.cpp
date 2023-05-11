
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <vector>

#include "test_runner.h"
using namespace std;

#if (1)
template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
    auto res = find_if(first, last, pred);
    if (res != last) {
        for (auto it = res; it != last; it = find_if(next(it), last, pred)) {
            if (it != last && *res < *it) {
                res = it;
            }
        }
    }
    return res;
}
#else
template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
    auto mx{last};
    for (auto it = first; it != last; ++it) {
        if (pred(*it) && (mx == last || *mx < *it)) {
            mx = it;
        }
    }
    return mx;
}
#endif
