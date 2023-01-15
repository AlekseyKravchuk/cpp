#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWordsView(std::string_view strStrView) {
    vector<string_view> result;

    // устанавливаем позицию на первый непробельный символ
    size_t pos = strStrView.find_first_not_of(" ");
    if (pos == std::string_view::npos) {
        return result;
    }

    // pos - это позиция первого непробельного символа и по совместительству - длина префикса, подлежащего удалению
    strStrView.remove_prefix(pos);

    // теперь начинаем искать пробел в "strStrView" при условии, что она начинается с непробельного символа
    while (true) {
        // ищем пробел в ещё не обработанной части
        pos = strStrView.find(' ');

        // если пробел нашелся, тогда "pos" - это его позиция, а так же расстояние до него (т.е. длина обработанного слова),
        // а если пробел НЕ нашелся, то substr будет вызвана от (0, npos), и это будет вся оставшаяся подстрока
        // string_view::substr(0, npos) - это вся строка, начиная с текущей (в данном случае, нулевой) позиции
        // вызов "strStrView.substr(0, pos)" означает, что нужно вернуть подстроку, начинающуюся с позиции "0" и имеющую длину "pos"
        result.push_back(strStrView.substr(0, pos));

        // если пробел не найден => все слова в строке найдены, завершаем цикл
        if (pos == strStrView.npos) {
            break;
        } else {
            // ищем первый непробельный символ
            size_t firstNonSpace = strStrView.find_first_not_of(" ", pos);

            // если первый непробельный символ не найден, значит за уже обработанным (сохраеннным) словом следуют ТОЛЬКО пробельные символы
            // => завершаем цикл
            if (firstNonSpace == strStrView.npos) {
                break;
            }


            // если firstNonSpace и (pos+1) совпадают, значит пробел в единственном экземпляре
            if (firstNonSpace == (pos+1)) {
                // "откусываем" от "strStrView" префикс, включающий в себя обработанное слово и 1 пробел
                strStrView.remove_prefix(pos + 1);
            } else {
                // в противном случае "откусываем" от "strStrView" префикс, включающий в себя обработанное слово и N пробелов
                strStrView.remove_prefix(firstNonSpace);
            }
        }
    }

    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}
