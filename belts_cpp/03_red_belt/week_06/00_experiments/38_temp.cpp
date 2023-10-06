// word_5 : {
//          {doc_id_1, number_of_occurrences_1},
//          {doc_id_2, number_of_occurrences_2},
//          {doc_id_4, number_of_occurrences_6} }

// ...

// word_6 : {
//          {doc_id_1, number_of_occurrences_1},
//          {doc_id_3, number_of_occurrences_2},
//          {doc_id_5, number_of_occurrences_6} }

// ...

// word_1 : {
//          {doc_id_5, number_of_occurrences_7}
//                                               }


// Запрос состоит из 3-х слов: {word_5, word_1, word_6}

#include <queue>  // std::priority_queue
// для каждого набора слов-запросов будет создаваться вот такой std::map:
std::unordered_map<size_t, size_t> docid_count;

for (auto& word : {word_5, word_1, word_6}) {
    for (auto [docid, count] : _index[word]) {
        docid_count[docid] += count;
    }
}


// Мне нужно обработать мой "std::unordered_map<std::string_view, std::map<size_t, size_t>> _index";
// Таким образом, чтобы на выходе получить словарь вида:
//             number_of_occurrences(word_5) + number_of_occurrences(word_6) + number_of_occurrences(word_1) : doc_id_5
//             number_of_occurrences(word_5) + number_of_occurrences(word_6) + number_of_occurrences(word_1) : doc_id_3