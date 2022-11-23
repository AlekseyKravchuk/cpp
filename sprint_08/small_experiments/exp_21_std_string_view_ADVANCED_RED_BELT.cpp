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

// В предыдущей реализации "SplitIntoWordsView" мы поддерживали текущую позицию в переменной "pos" и двигали её по "string_view"
// В этой версии избавимся от этой переменной - текущей позиции "pos".
// Вместо того, чтобы держать единый "string_view" и по нему ходить позицией "pos", будем двигать начало "string_view" после каждого
// обработанного слова. Теперь в каждый момент наш "string_view" будет указывать на ДИАПАЗОН ЕЩЁ НЕ ОБРАБОТАННЫХ СИМВОЛОВ.
vector<string_view> SplitIntoWordsView(const string& s) {
    vector<string_view> result;

    // создаем более быструю строку "string_view" (за счет того, что она не копирует символы)
    string_view strStrView = s;

    // теперь начинаем искать пробел в "strStrView", начиная с позиции "pos"
    while (true) {
        // ищем пробел в ещё не обработанной части
        size_t spacePos = strStrView.find(' ');

        // берем подстроку от нулевой позиции до пробела, определяемого позицией "spacePos"
        // если пробел нашелся, тогда "spacePos" - это его позиция, а так же расстояние до него (т.е. длина обработанного слова),
        // а если пробел НЕ нашелся, то substr будет вызвана от (0, npos), и это будет вся оставшаяся строка
        // string_view::substr(0, npos) - это вся строка, начиная с текущей позиции
        // вызов "strStrView.substr(0, spacePos)" означает, что нужно вернуть подстроку, начинающуюся с позиции "0" и имеющую длину "spacePos"
        result.push_back(strStrView.substr(0, spacePos));

        // если пробел не найден => все слова в строке найдены
        if (spacePos == strStrView.npos) {
            break;
        } else {
            // нужно "откусить" от начала "strStrView" уже обработанный кусок длины "(spacePos + 1)" - префикс
            // с помощью специального метода "std::string_view::remove_prefix"
            // в вызове метода указываем длину префикса, который нужно "откусить"
            strStrView.remove_prefix(spacePos + 1);
        }
    }

    return result;
}

// можно сделать более компактно, просто передав в функцию непосредственно std::string_view по значению!!!
// по сути, в эту функцию можно передать и строку - "std::string_view" будет создан конструктором
// Компилятор умеет превращать строки в объекты типа "string_view".
// Это значит, что в функцию, ожидающую "string_view", можно передать и строку.
// Все преобразования будут выполнены автоматически.
// Но если функция ожидает аргумент-строку, передать туда "string_view" уже не удастся —
// обратное преобразование из "string_view" в "string" возможно только как явное.
vector<string_view> SplitIntoWordsViewAdvanced(std::string_view strStrView) {
    vector<string_view> result;

    // теперь начинаем искать пробел в "strStrView", начиная с позиции "pos"
    while (true) {
        // ищем пробел в ещё не обработанной части
        size_t spacePos = strStrView.find(' ');

        // берем подстроку от нулевой позиции до пробела, определяемого позицией "spacePos"
        // если пробел нашелся, тогда "spacePos" - это его позиция, а так же расстояние до него (т.е. длина обработанного слова),
        // а если пробел НЕ нашелся, то substr будет вызвана от (0, npos), и это будет вся оставшаяся строка
        // string_view::substr(0, npos) - это вся строка, начиная с текущей позиции
        // вызов "strStrView.substr(0, spacePos)" означает, что нужно вернуть подстроку, начинающуюся с позиции "0" и имеющую длину "spacePos"
        result.push_back(strStrView.substr(0, spacePos));

        // если пробел не найден => все слова в строке найдены
        if (spacePos == strStrView.npos) {
            break;
        } else {
            // нужно "откусить" от начала "strStrView" уже обработанный кусок длины "(spacePos + 1)" - префикс
            // с помощью специального метода "std::string_view::remove_prefix"
            // в вызове метода указываем длину префикса, который нужно "откусить"
            strStrView.remove_prefix(spacePos + 1);
        }
    }

    return result;
}

int main() {
    const string stringToBeSplited = "some words to be splitted"s;
    // const string stringToBeSplited = "some           words        to     be          splitted"s;
    // vector<string> splitted = SplitIntoWords(stringToBeSplited);

    // vector<string_view> splitted = SplitIntoWordsView(stringToBeSplited);

    vector<string_view> splitted = SplitIntoWordsViewAdvanced(stringToBeSplited);

    PrintRange(begin(splitted), end(splitted));

    return 0;
}
