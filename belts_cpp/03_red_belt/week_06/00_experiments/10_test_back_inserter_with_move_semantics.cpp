#include <algorithm>  // std::equal
#include <cassert>
#include <iterator>  // std::back_inserter
#include <utility>   // std::move
#include <vector>

#include "../../print.h"

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

std::ostream& operator<<(std::ostream& os, NoncopyableInt& number) {
    return os << number.value;
}

bool operator==(NoncopyableInt& lhs, NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

int main() {
    std::vector<NoncopyableInt> source;
    source.push_back({2});
    source.push_back({3});
    source.push_back({4});
    source.push_back({5});
    source.push_back({6});
    source.push_back({7});

    std::vector<NoncopyableInt> target;
    target.push_back({11});
    target.push_back({22});
    target.push_back({33});
    target.push_back({44});

    // нужно ПЕРЕМЕСТИТЬ в конец вектора "target" элементы [3, 4, 5] с индексами [1, 2, 3] соответственно
    std::move(source.begin() + 1, source.begin() + 4,
              std::back_inserter(target));

    // проверяем, что происходит именно перемещение, а не копирование!!!
    // этот код не должен скомпилироваться!!!
    // std::copy(source.begin() + 1, source.begin() + 4,
    //           std::back_inserter(target));

    // target.resize(target.size() + 3);
    // std::move(source.begin() + 1, source.begin() + 4,
    //           target.end());

    // std::copy(std::make_move_iterator(source.begin() + 1), std::make_move_iterator(source.begin() + 4),
    //           std::back_inserter(target));

    std::vector<NoncopyableInt> expected_target;
    expected_target.push_back({11});
    expected_target.push_back({22});
    expected_target.push_back({33});
    expected_target.push_back({44});
    expected_target.push_back({3});
    expected_target.push_back({4});
    expected_target.push_back({5});

    assert(std::equal(target.begin(), target.end(),
                      expected_target.begin()));

    // std::cout << "source after moving: " << source << std::endl;
    // std::cout << "target after moving: " << target << std::endl;

    return 0;
}
