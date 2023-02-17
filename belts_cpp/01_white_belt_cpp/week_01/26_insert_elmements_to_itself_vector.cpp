#include <iostream>
#include <iterator>  // std::make_move_iterator
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    /*
    Задача: требуется переместить все строки, содержащиеся в последних delta = 2 векторах, нужно переместить
    в  вектор строк v[v.size() -1 - delta]
    */
    std::vector<std::vector<std::string>> v{
        /* 0 */ {"abc"s, "dfg"s, "hijk"s},
        /* 1 */ {"111"s, "222"s, "333"s},
        /* 2 */ {"aba"s, "caba"s, "boom"s},
        /* 3 */ {"some"s, "any"s, "test"s},  // <= в конец этого вектора строк нужно вставить элементы из всех последующих векторов
        /* 4 */ {"joy"s, "case"s, "if"s},
        /* 5 */ {"mother"s, "father"s, "sister"s, "brother"s}};

    int delta = 2;
    auto dst_idx = v.size() - 1 - delta;

    for (auto i = v.size() - delta; i < v.size(); ++i) {
        std::cout << "i = " << i << std::endl;

        auto it_begin = std::make_move_iterator(v[i].begin());
        auto it_end = std::make_move_iterator(v[i].end());

        // std::copy(it_begin, it_end, std::back_inserter(v[dst_idx]));

        v[dst_idx].reserve(v[dst_idx].size() + v[i].size());
        v[dst_idx].insert(v[dst_idx].end(), it_begin, it_end);
    }

    v.resize(v.size() - delta);

    return 0;
}
