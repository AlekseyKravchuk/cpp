#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std::literals;

/*

Задание по программированию «Синонимы»

Два слова называются синонимами друг друга, если они имеют похожие значения. Реализуйте следующие операции над словарём синонимов:

    ADD word1 word2 — добавить в словарь пару синонимов (word1, word2).

    COUNT word — узнать количество синонимов слова word.

    CHECK word1 word2 — проверить, являются ли слова word1 и word2 синонимами. Слова word1 и word2считаются синонимами, если среди запросов ADD был хотя бы один запрос ADD word1word2 или ADD word2word1.

Замечание:

Для упрощения, будем считать, что синонимы не обладают транзитивностью, то есть, если A - синоним B, а B - синоним C, то из этого НЕ следует, что A - синоним C.
Формат ввода:
    - Сначала вводится количество запросов Q, затем Q строк с описаниями запросов.
    - Гарантируется, что в каждом запросе CHECK и ADD слова word1 и word2 различны.
    - Все слова состоят лишь из латинских букв, цифр и символов подчёркивания.

Формат вывода:
Для каждого запроса в соответствующей строке выведите ответ на него:
    - В ответ на запрос COUNT word выведите единственное целое число — количество синонимов слова word.
    - В ответ на запрос CHECK word1word2 выведите строку YES, если word1 и word2 являются синонимами, и NO в противном случае.

Пример

Ввод:
8
ADD program code
COUNT cipher
ADD code cipher
COUNT code
COUNT program
CHECK code program
CHECK program cipher
CHECK cpp java


Вывод:
0
2
1
YES
NO
NO
*/

void ProcessOperations(int n) {
    using std::cin;
    using std::cout;
    using std::endl;
    using std::map;
    using std::set;
    using std::string;

    enum class OPERATIONS {
        ADD,
        COUNT,
        CHECK
    };

    map<string, OPERATIONS> str2op = {
        {"ADD", OPERATIONS::ADD},
        {"COUNT", OPERATIONS::COUNT},
        {"CHECK", OPERATIONS::CHECK}};

    string op_as_str{};
    map<string, set<string>> synonyms;

    for (int i = 0; i < n; ++i) {
        cin >> op_as_str;

        switch (str2op[op_as_str]) {
            case OPERATIONS::ADD: {
                string word1{}, word2{};
                cin >> word1 >> word2;

                // второе слово добавляем в список (множество) синонимов первого слова
                synonyms[word1].insert(word2);

                // и наоборот: первое слово добавляем в список синонимов второго слова
                synonyms[word2].insert(word1);

                break;
            }
            case OPERATIONS::COUNT: {
                string word;
                cin >> word;

                cout << synonyms[word].size() << endl;

                break;
            }
            case OPERATIONS::CHECK: {
                string word1{}, word2{};
                cin >> word1 >> word2;

                // ищем второе слово во множестве синонимов первого слова (можно было реализовать и наоборот)
                if (synonyms[word1].count(word2)) {
                    cout << "YES" << endl;
                } else {
                    cout << "NO" << endl;
                }

                break;
            }
            default:
                break;
        }
    }
}

int main() {
    // std::ifstream in("34_task_input.txt");      // configuring input from the file "34_task_input.txtt"
    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "34_task_input.txt"

    int n{};
    std::cin >> n;
    ProcessOperations(n);

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
