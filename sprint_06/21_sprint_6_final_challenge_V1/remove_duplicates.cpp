#include "remove_duplicates.h"

// функция для поиска и удаления дубликатов
void RemoveDuplicates(SearchServer& searchSrv) {
    // заводим вектор под идентификаторы дублирующихся документов
    std::vector<int> duplicatesIDs;

    for (auto it = searchSrv._hash_docID.begin(); it != searchSrv._hash_docID.end();) {
        // выбираем первое хеш-значение (хеш-значения в std::multimap отсортированы по возрастанию)
        size_t checkedHash = it->first;

        // ищем диапазон с одинаковыми значениями хешей
        auto [lower, upper] = searchSrv._hash_docID.equal_range(checkedHash);

        // если дубликатов не найдено, переходим к следующему значению хеша
        if (std::distance(lower, upper) == 1) {
            it = upper;
            continue;
        }

        // создаем "vectorToSaveOrdering" и добавляем в него docID с одинаковыми значениями хешей 
        std::vector<int> vectorToSaveOrdering;
        for (auto itHash = lower; itHash != upper; ++itHash) {
            vectorToSaveOrdering.push_back(itHash->second);
        }

        // сортируем получившийся поддиапазон идентификаторов дублирующихся документов
        std::sort(vectorToSaveOrdering.begin(), vectorToSaveOrdering.end());

        // добавляем в итоговый вектор "duplicatesIDs" отсортированные поддиапазоны без наименьшего docID в каждом из них
        duplicatesIDs.insert(duplicatesIDs.end(), std::next(vectorToSaveOrdering.begin()), vectorToSaveOrdering.end());


        // переводим итератор на следующий диапазон хешей
        it = upper;
    }

    // снова сортируем по возрастанию итоговый вектор дубликатов, которые подлежат удалению
    std::sort(duplicatesIDs.begin(), duplicatesIDs.end());

    // обрабатываем и удаляем дубликаты
    for (auto itDocID = duplicatesIDs.begin(); itDocID != duplicatesIDs.end(); ++itDocID) {
        std::cout << "Found duplicate document id " << *itDocID << std::endl;
        searchSrv.RemoveDocument(*itDocID);
    }
}