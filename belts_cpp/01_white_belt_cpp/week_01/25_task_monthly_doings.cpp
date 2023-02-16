#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/*
Задание по программированию «Ежемесячные дела»

У каждого из нас есть повторяющиеся ежемесячные дела, каждое из которых нужно выполнять в конкретный день каждого месяца:
оплата счетов за электричество, абонентская плата за связь и пр.
Вам нужно реализовать работу со списком таких дел на месяц, а именно, реализовать набор следующих операций:

ADD i s <=> Назначить дело с названием s на день i текущего месяца.

DUMP i  <=> Вывести все дела, запланированные на день i текущего месяца.

NEXT    <=> Перейти к списку дел на новый месяц.
            При выполнении данной команды вместо текущего (старого) списка дел на текущий месяц
            создаётся и становится активным (новый) список дел на следующий месяц:
            все дела со старого списка дел копируются в новый список.
            После выполнения данной команды новый список дел и следующий месяц становятся текущими, а работа со старым списком дел прекращается.
            При переходе к новому месяцу необходимо обратить внимание на разное количество дней в месяцах:
                если следующий месяц имеет больше дней, чем текущий, «дополнительные» дни необходимо оставить пустыми (не содержащими дел);
                если следующий месяц имеет меньше дней, чем текущий, меньше дней, дела со всех «лишних» дней необходимо переместить на последний день следующего месяца.

Замечания:

    Историю списков дел хранить не требуется, работа ведется только с текущим списком дел текущего месяца.
    Более того, при создании списка дел на следующий месяц, он «перетирает» предыдущий список.

    Обратите внимание, что количество команд NEXT в общей последовательности команд при работе со списком дел может превышать 11.

    Начальным текущим месяцем считается январь.

    Количества дней в месяцах соответствуют Григорианскому календарю с той лишь разницей, что в феврале всегда 28 дней.

Формат ввода:
    Сначала число операций Q, затем описания операций.
    Названия дел s уникальны и состоят только из латинских букв, цифр и символов подчёркивания.
    Номера дней i являются целыми числами и нумеруются от 1 до размера текущего месяца.

Формат вывода:
    Для каждой операции типа DUMP в отдельной строке выведите количество дел в соответствующий день, а затем их названия, разделяя их пробелом. Порядок вывода дел в рамках каждой операции значения не имеет.

Пример:

Ввод:
12
ADD 5 Salary
ADD 31 Walk
ADD 30 WalkPreparations
NEXT
DUMP 5
DUMP 28
NEXT
DUMP 31
DUMP 30
DUMP 28
ADD 28 Payment
DUMP 28

Вывод:
1 Salary
2 WalkPreparations Walk
0
0
2 WalkPreparations Walk
3 WalkPreparations Walk Payment


Указание:
    Для дописывания всех элементов вектора v2 в конец вектора v1 удобно использовать метод insert:
    v1.insert(end(v1), begin(v2), end(v2));
 */

enum class OPERATIONS { ADD,
                        DUMP,
                        NEXT
};

std::map<std::string, OPERATIONS> str2op = {
    {"ADD", OPERATIONS::ADD},
    {"DUMP", OPERATIONS::DUMP},
    {"NEXT", OPERATIONS::NEXT}};

template <typename Collection>
void PrintCollection(const Collection& collection) {
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << ' ' << elm;
        }
    }
    std::cout << std::endl;
}

void ProcessOperations(int n) {
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int month_index{0};
    std::vector<std::vector<std::string>> v(months[month_index]);
    std::string op_as_str;
    int day_num{};
    std::string task_name{};

    for (int i = 0; i < n; ++i) {
        std::cin >> op_as_str;
        switch (str2op[op_as_str]) {
            case OPERATIONS::ADD: {
                std::cin >> day_num;
                std::cin >> task_name;
                v[day_num].push_back(task_name);
                break;
            }
            case OPERATIONS::DUMP: {
                std::cin >> day_num;
                PrintCollection(v[day_num]);
                break;
            }
            case OPERATIONS::NEXT: {
                int next_index = (month_index + 1) % std::size(months);
                int delta = months[next_index] - months[month_index];

                // если в следующем месяце дней больше, чем в текущем,
                // «дополнительные» дни оставляем пустыми (не содержащими дел);
                if (delta > 0) {
                    v.resize(v.size() + delta, std::vector<std::string>());
                    // если следующий месяц имеет меньше дней, чем текущий,
                    // дела со всех «лишних» дней необходимо переместить на последний день следующего месяца.
                } else if (delta < 0) {
                    // TODO:
                    std::vector<std::vector<std::string>> next_v;
                    next_v.reserve(v.size() - delta);
                    std::copy(v.begin(), v.end() - delta, next_v.begin());
                }
                month_index = next_index;
                break;
            }
            default:
                break;
        }
    }
}

int main() {
    int n{};
    std::cin >> n;
    ProcessOperations(n);

    return 0;
}
