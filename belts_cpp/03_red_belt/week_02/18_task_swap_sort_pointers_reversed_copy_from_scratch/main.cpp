#include <algorithm>  // std::max, std::sort
#include <cstddef>    // ptrdiff_t
#include <string>
#include <utility>  // std::move
#include <vector>

#include "test_runner.h"
#include "tests.h"

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestSortPointers);

    RUN_TEST(tr, TestReverseCopyStepik);

    RUN_TEST(tr, TestReverseCopy_Non_Overlapping_src_dst);
    RUN_TEST(tr, TestReverseCopy_Non_Overlapping_dst_src);

    RUN_TEST(tr, TestReverseCopy_Overlapping_src_dst);
    RUN_TEST(tr, TestReverseCopy_Overlapping_dst_src);
    // RUN_TEST(tr, TestReverseCopy_Overlapping_dst_equals_src);

    return 0;
}
