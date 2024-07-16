#pragma once

#include <vector>
#include <cstddef>

std::vector<int> GenerateRandomVectorOfInt(int sample_size = 10'000'000, int seed = 93);

void CountingSort_SingleThread(std::vector<int>::iterator first,
                               std::vector<int>::iterator second,
                               std::vector<unsigned int>::iterator it_counter_begin,
                               int min_val,
                               int max_val);