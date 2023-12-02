/*
Представим, что мы разрабатываем ПО для банка.
Нам поставили задачу разработать класс "Account", который будет представлять собой банковский счет.
Требования к этому классу достаточно простые:
    - он представляет собой банковский счет;
    - не должен позволять тратить больше денег, чем есть на счету;
    - не должен допускать, чтобы баланс счета стал отрицательным (обеспечить, чтобы клиент не смог "уйти в минус").
*/

#include <algorithm>
#include <functional> // std::ref
#include <future>
#include <iomanip>
#include <iostream>
#include <vector>

#include "profile.h"

struct Account {
    int balance = 0;
    // std::vector<int> history = {};  // добавляем новый функционал - ведение истории списания со счета

    bool Spend(int value) { // "value" - это сумма, которую мы хотим потратить с нашего счета
        if (value <= balance) {
            balance -= value;
            // history.push_back(value);  // здесь случится "malloc(): corrupted top size"
            return true; // да, мы правда смогли потратить деньги в размере "value"
        }

        return false;
    }
};

// хотим проверить, что требования к нашему классу выполняются
int SpendMoney(Account& account) {
    int total_spent = 0;

    // 100'000 пытаемся потратить 1 рубль
    for (int i = 0; i < 100'000; ++i) {
        if (account.Spend(1)) {
            ++total_spent;
        }
    }

    return total_spent;
}

int main() {
    Account family_account{100'000};

    // все члены семьи параллельно (асинхронно) тратять деньги с одного СЕМЕЙНОГО счета, пользуясь своими банковскими картами
    auto husband = std::async(SpendMoney, std::ref(family_account));
    auto wife = std::async(SpendMoney, std::ref(family_account));
    auto son = std::async(SpendMoney, std::ref(family_account));
    auto daughter = std::async(SpendMoney, std::ref(family_account));

    int spent = husband.get() + wife.get() + son.get() + daughter.get();

    std::cout << "Total spent: " << spent << std::endl
              << "Balance: " << family_account.balance << std::endl;
    /*
    Запуск №1:
        Total spent: 289183
        Balance: -1
    Запуск №2:
        Total spent: 282073
        Balance: 0
    */

    return 0;
}
