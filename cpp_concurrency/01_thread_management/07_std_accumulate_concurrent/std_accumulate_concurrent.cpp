#include <iostream>
#include <vector>
#include <numeric>     // std::accumulate
#include <utility>     // std::move
#include <string>      // std::to_string
#include <iterator>    // std::next, std::distance
#include <thread>      // std::thread
#include <algorithm>   // std::min
#include <functional>  // std::mem_fn
#include <cstdlib>     // std::srand

using namespace std;

const int MIN_BLOCK_SIZE = 1'000;

// in this overloading reference is used as transferring mechanism
template <typename iterator, typename T>
void accumulate(iterator start, iterator end, T& ref) {
    ref = std::accumulate(start, end, 0);
}

// TODO:
template <typename iterator, typename T>
T parallel_accumulate(iterator start, iterator end, T& ref) {
    // определяем количество элементов, над которыми нужно выполнить "accumulate"
    int input_size = std::distance(start, end);
    int allowed_threads_by_elements_count = input_size / MIN_BLOCK_SIZE;

    // также принимаем во внимание значение "std::thread::hardware_concurrency"
    int allowed_threads_by_hardware = std::thread::hardware_concurrency();

    int num_threads = std::min(allowed_threads_by_elements_count, allowed_threads_by_hardware);

    int block_size = (input_size + 1) / num_threads;

    std::vector<T> results(num_threads);

    // we are going to accumulate final data block in the calling thread => so substract 1 thread
    std::vector<std::thread> threads(num_threads-1);

    iterator last;

    for (int i = 0; i < num_threads-1; ++i) {
        last = start;
        std::advance(last, block_size);
        threads[i] = std::thread(accumulate<iterator, T>,
                                 start,
                                 last,
                                 std::ref(results[i]));
        start = last;
    }

    results[num_threads-1] = std::accumulate(start, end, 0);
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(), results.end(), ref);
}

int main() {
    const int size = 8'000;
    int* my_array = new int[size];
    int ref = 0;
    std::srand(0);

    for (size_t i = 0; i < size; ++i) {
        my_array[i] = rand() % 10;
    }

    int rer_val = parallel_accumulate<int*, int>(my_array, my_array + size, ref);
    cout << "Accumulated value : " << rer_val << endl;

    return 0;
}
