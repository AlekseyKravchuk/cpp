#include <any>
#include <chrono>
#include <concepts>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#ifndef __cpp_lib_format
// std::format polyfill using fmtlib
#include <fmt/core.h> // Install "libfmt-dev" package that is available on debian/ubuntu: sudo apt install libfmt-dev
namespace std {
using fmt::format;
}
#else
#include <format>
#endif

namespace n701 {

struct game_unit {
    virtual void attack() = 0;
};

// knight - рыцарь
struct knight : game_unit {
    virtual void attack() override {
        std::cout << "draw sword\n";
    }
};

// mage - маг, волшебник
struct mage : game_unit {
    virtual void attack() override {
        // произнести магическое заклятие
        std::cout << "spell magic curse\n";
    }
};

void fight(std::vector<game_unit*> const& units) {
    for (auto unit : units) {
        unit->attack();
    }
}

struct knight_mage : game_unit {
    void attack() override {
        // обнажить волшебный меч
        std::cout << "draw magic sword\n";
    }
};

struct attack {
    int value;
};

struct defence {
    int value;
};

attack operator+(attack const& a, int value) {
    return attack{a.value + value};
}

knight_mage operator+(knight const& k, mage const& m) {
    return knight_mage{};
}

void increment(attack& a) { a.value++; }
void increment(defence& d) { d.value++; }

// template <typename T>
// void increment(T& t) { t.value++; }
} // namespace n701

namespace n702 {
// Перепишем предыдущий пример (с использованием виртуальных функций) за счет использования CRTP
// Класс(структура) "game_unit" превратилась в шаблон класса, который содержит фунцию-член "do_attack()"
template <typename T>
struct game_unit {
    void attack() {
        static_cast<T*>(this)->do_attack();
    }
};

struct knight : public game_unit<knight> {
    void do_attack() {
        std::cout << "draw sword\n";
    }
};

struct mage : public game_unit<mage> {
    void do_attack() {
        // произнести магическое заклятие
        std::cout << "spell magic curse\n";
    }
};

template <typename T>
void fight(const std::vector<game_unit<T>*>& units) {
    for (auto unit : units) {
        unit->attack();
    }
}

} // namespace n702

int main() {
    // {
    //     using namespace n701;

    //     knight k;
    //     mage m;
    //     fight({&k, &m});

    //     knight_mage km = k + m;
    //     km.attack();

    //     attack a{42};
    //     a = a + 2;

    //     defence d{50};
    //     increment(a);
    //     increment(d);
    // }

    {
        using namespace n702;
        knight k;
        mage m;
        // fight({&k, &m});  // теперь мы не можем использовать функцию "fight" как ранее в примере с вирт.функциями

        // Мы сменили полиморфизм времени выполнения на полиморфизм времени компиляции,
        // поэтому функция "fight" не может обрабатывать объекты типов "knight" и "mage" полиморфно.
        // Вместо этого мы получили 2 различные перегрузки:
        // 1-ая может обрабатывать объекты типа "knight",
        // 2-ая может обрабатывать объекты типа "mage".
        fight<knight>({ &k });
        fight<mage>({ &m });
    }
}