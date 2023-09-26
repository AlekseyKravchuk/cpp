#include <cstddef>  // size_t
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <vector>

#include "../../print.h"

struct Range {
    int min{};
    int max{};
    size_t size{};

    Range(int start_range, int end_range)
        : min(start_range),
          max(end_range),
          size(max - min + 1) {}
};

size_t GetNumOfElementsPerBucket(size_t range_size, size_t num_of_buckets) {
    return (range_size % num_of_buckets == 0)
               ? range_size / num_of_buckets
               : range_size / num_of_buckets + 1;
}

std::map<int, int> GetMapPagesIndexer(Range range, size_t num_of_buckets) {
    std::map<int, int> map_pg_indexer;
    int border = range.min;
    int elements_per_bucket = GetNumOfElementsPerBucket(range.size, num_of_buckets);

    for (size_t i = 0; i < num_of_buckets; ++i) {
        map_pg_indexer[border] = i;
        border += elements_per_bucket;
    }

    return map_pg_indexer;
}

// class TrickyMap {
//    private:
//     struct Range {
//         int min{};
//         int max{};
//         size_t size{};

//         Range(int start_range = int{}, int end_range = int{})
//             : min(start_range),
//               max(end_range),
//               size(max - min + 1) {}
//     };
//     Range range{};
//     size_t _num_of_buckets;
//     std::vector<std::map<int, int>> _subdict_store(_num_of_buckets);
// };

// returns index of subdict within subdict_store ("vector<map<int, int>>")
int GetSubDictStorageId(std::map<int, int> map_pg_indexer,
                        int key) {
    if (map_pg_indexer.count(key)) {
        return map_pg_indexer[key];
    }

    const auto it = map_pg_indexer.upper_bound(key);

    return std::prev(it)->second;
}

void InsertIntoSubDict(std::vector<std::map<int, int>>& subdict_store,
                       std::map<int, int>& map_pg_indexer,
                       int key,
                       int value) {
    // ПОКА предполагаем, что подаваемые на вход ключи, "key", в словаре ОТСУТСТВУЮТ
    // => просто ДОБАВЛЯЕМ пару <key, value> в соответствующий словарь "subdict_store[map_pg_indexer[key]]";
    int i = GetSubDictStorageId(map_pg_indexer, key);
    subdict_store[i][key] = value;
    std::cout << "Inserted <" << key << ", " << value
              << "> into bucket[" << i << "]" << std::endl;
}

std::map<int, int> MergeSubDicts(std::vector<std::map<int, int>>& subdict_store) {
    if (!subdict_store.size()) {
        return {};
    }

    if (subdict_store.size() == 1ul) {
        return subdict_store[0];
    }

    std::map<int, int> merged_map;

    for (auto& sub_dict : subdict_store) {
        merged_map.merge(sub_dict);
    }

    return merged_map;
}

int main() {
    int min = -10;
    int max = 10;

    // int min = 3;
    // int max = 5;
    // std::cout << "range_size of [" << min << "; " << max << "] = " << Range{min, max}.size << " elements;" << std::endl;

    size_t num_of_buckets = 4;
    std::map<int, int> map_pg_indexer = GetMapPagesIndexer(Range(min, max), num_of_buckets);

    std::cout << map_pg_indexer << std::endl;  // {(-10, 0) (-4, 1) (2, 2) (8, 3)} <== OK

    std::vector<std::map<int, int>> subdict_store(num_of_buckets);

    int id_0 = GetSubDictStorageId(map_pg_indexer, -9);
    std::cout << "id_0 = " << id_0 << std::endl;  // 0 <= OK

    int id_1 = GetSubDictStorageId(map_pg_indexer, -5);
    std::cout << "id_1 = " << id_1 << std::endl;  // 0 <= OK

    int id_2 = GetSubDictStorageId(map_pg_indexer, -4);
    std::cout << "id_2 = " << id_2 << std::endl;  // 1 <= OK

    int id_3 = GetSubDictStorageId(map_pg_indexer, 0);
    std::cout << "id_3 = " << id_3 << std::endl;  // 1 <= OK

    int id_4 = GetSubDictStorageId(map_pg_indexer, 1);
    std::cout << "id_4 = " << id_4 << std::endl;  // 1 <= OK

    int id_5 = GetSubDictStorageId(map_pg_indexer, 2);
    std::cout << "id_5 = " << id_5 << std::endl;  // 2 <= OK

    int id_6 = GetSubDictStorageId(map_pg_indexer, 4);
    std::cout << "id_6 = " << id_6 << std::endl;  // 2 <= OK

    int id_7 = GetSubDictStorageId(map_pg_indexer, 7);
    std::cout << "id_7 = " << id_7 << std::endl;  // 2 <= OK

    int id_8 = GetSubDictStorageId(map_pg_indexer, 8);
    std::cout << "id_8 = " << id_8 << std::endl;  // 3 <= OK

    int id_9 = GetSubDictStorageId(map_pg_indexer, 9);
    std::cout << "id_9 = " << id_9 << std::endl;  // 3 <= OK

    int id_10 = GetSubDictStorageId(map_pg_indexer, 10);
    std::cout << "id_10 = " << id_10 << std::endl;  // 3 <= OK

    auto [key1, value1] = std::pair<int, int>{-2, 150};
    InsertIntoSubDict(subdict_store,
                      map_pg_indexer,
                      key1,
                      value1);

    auto [key2, value2] = std::pair<int, int>{-7, 22};
    InsertIntoSubDict(subdict_store,
                      map_pg_indexer,
                      key2,
                      value2);

    auto [key3, value3] = std::pair<int, int>{9, 333};
    InsertIntoSubDict(subdict_store,
                      map_pg_indexer,
                      key3,
                      value3);

    auto [key4, value4] = std::pair<int, int>{5, 222};
    InsertIntoSubDict(subdict_store,
                      map_pg_indexer,
                      key4,
                      value4);

    auto merged_map = MergeSubDicts(subdict_store);
    std::cout << merged_map << std::endl;

    std::cout << "At the end of main" << std::endl;

    return 0;
}
