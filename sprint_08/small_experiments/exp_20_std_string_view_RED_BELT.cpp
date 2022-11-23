#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

// Функция корректно разбивает строку на слова только в том случае, когда слова разделены одиночными пробелами
vector<string> SplitIntoWords(const string& str) {
    vector<string> result;

    auto str_begin = str.begin();
    const auto str_end = str.end();

    while (true) {
        auto it = std::find(str_begin, str_end, ' ');

        // помещаем подстроку в вектор за счет использования конструктора строки от двух итераторов
        result.push_back(string(str_begin, it));

        if (it == str_end) {
            break;
        } else {
            str_begin = it + 1;
        }
    }

    return result;
}

vector<string_view> SplitIntoWordsView(const string& s) {
    vector<string_view> result;

    // создаем более быструю строку "string_view" (за счет того, что она не копирует символы)
    string_view strStrView = s;

    // ранее мы работали с итераторами - использовали алгоритм std::find, использовали конструктор строки от двух итераторов
    // теперь у нас будет "std::string_view", а "string_view" с итераторами не дружит, т.к. "string_view" хочет ссылаться
    // на ПОДРЯД ИДУЩИЙ диапазон символов, а итераторы не могут этого обеспечить,
    /// именно поэтому "string_view" работает с позициями в строках, а не с итераторами

    // начинаем с нулевой позиции в строке (а не с итератора)
    size_t pos = 0;

    // аналог "итератору за концом контейнера" является "позиция за концом контейнера": "std::string::npos"
    // "std::string::npos" - это некоторое очень большое число, обозначающее несуществующую позицию
    size_t posPastTheEnd = std::string::npos;

    // теперь начинаем искать пробел в "strStrView", начиная с позиции "pos"
    while (true) {
        // заводим переменную типа "size_t", храняющую позицию пробела
        // вызываем метод std::string_view::find("что_искать", "начиная_с_какой_позиции_искать")
        size_t spPos = strStrView.find(' ', pos);

        // теперь в "result" нужно положить подстроку (std::string_view), начиная с позиции "pos" и заканчивая "spPos" (не включая)
        // для этого есть метод "std::string_view_substr", который возвращает "string_view" на искомую подстроку
        // 1-ый аргумент "std::string_view_substr" - это начальная позиция подстроки
        // 2-ой аргумент "std::string_view_substr" - это длина этой подстроки
        // для 2-ого аргумента можно было бы написать "(spPos - pos)", но нужно предусмотреть случай, когда "spacePos == npos"
        // это случай рассмотрим отдельно

        // "pos" - это текущая позиция
        // "spPos" - позиция найденного пробельного символа (если не нашли, то это "std::string::npos")
        // в случае, если пробел в диапазоне [pos, posPastTheEnd) не найден, помещаем в вектор всю подстроку:
        // "strStrView.substr(pos)" - подстрока от текущей позиции до конца
        // "strStrView.substr(pos, spPos - pos)" - подстрока от текущей позиции длины "(spPos - pos)"
        string_view substrToInsert = (spPos == posPastTheEnd) ? strStrView.substr(pos) : strStrView.substr(pos, spPos - pos);
        result.push_back(substrToInsert);

        // если пробел не найден => все слова в строке найдены
        if (spPos == posPastTheEnd) {
            break;
        } else {
            pos = spPos + 1;
        }
    }

    return result;
}

int main() {
    const string stringToBeSplited = "some words to be splitted"s;
    // const string stringToBeSplited = "some           words        to     be          splitted"s;
    // vector<string> splitted = SplitIntoWords(stringToBeSplited);

    vector<string_view> splitted = SplitIntoWordsView(stringToBeSplited);

    PrintRange(begin(splitted), end(splitted));

    return 0;
}
