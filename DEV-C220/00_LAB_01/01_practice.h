#pragma once

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <type_traits>  // std::decay
#include <utility>
#include <vector>

using namespace std::literals;

template <typename Collection>
void PrintCollection(const Collection& collection, const std::string& message = ""s) {
    if (!message.empty()) {
        std::cout << message;
    }

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

template <typename Collection>
void NegateAll(Collection& collection) {
    for (auto& elm : collection) {
        elm = -elm;
    }
}

std::string& operator-(std::string& str) {
    for (auto& ch : str) {
        if (std::isupper(ch)) {
            ch = std::tolower(ch);
        } else {
            ch = std::toupper(ch);
        }
    }

    return str;
}

template <typename Collection>
void absSort(Collection& collection) {
    std::sort(std::begin(collection), std::end(collection),
              [](const auto& lhs, const auto& rhs) { return std::abs(lhs) < std::abs(rhs); });
}

template <typename Collection_1, typename Collection_2>
auto SumCont(Collection_1& col_1, Collection_2& col_2) {

    // выводим тип вектора с помощью decltype и хитроумного выражения
    // размер результирующего вектора задаем как самый большой среди "col_1" и "col_2"
    // std::vector<decltype(*std::begin(col_1) + *std::begin(col_2))> target(std::max(std::size(col_1), std::size(col_2)));

    // упрощаем читаемость кода с помощью "using"
    using RetType = decltype(*std::begin(col_1) + *std::begin(col_2));
    const auto vecSize = std::max(std::size(col_1), std::size(col_2));
    
    std::vector<RetType> target(vecSize);

    // копируем все элементы из контейнера "col_1" (контейнер выбираем произвольно) в целевой контейнер "target"
    std::copy(std::begin(col_1), std::end(col_1), std::begin(target));

    // применяем БИНАРНУЮ "callable" к оставшемуся контейнеру ("col_2") и целевому ("target")
    // в качестве исходного диапазона берем ещё незадействованный контейнер "col_2"
    // поскольку размер "col_2" или равен размеру "target" или меньше
    std::transform(std::begin(col_2), std::end(col_2),
                   std::begin(target),
                   std::begin(target),
                   [](auto val1, auto val2) {
                       return val1 + val2;
                   });

    return target;
}

template <typename Collection_1, typename Collection_2, typename Collection_3, typename Predicate>
void Separate(Collection_1& src, Collection_2& dst_1, Collection_3& dst_2, Predicate pred) {
    std::copy_if(std::begin(src), std::end(src),
                 std::inserter(dst_1, std::end(dst_1)),
                 pred);

    std::remove_copy_if(std::begin(src), std::end(src),
                        std::inserter(dst_2, std::end(dst_2)),
                        pred);
}

// для задания 10

// generic variable template
enum class State : char { Idle,
                          Fidget,
                          Walk,
                          Scan,
                          Attack };

template <typename T>
std::map<std::string, T> myMap;

template <>
std::map<std::string, State> myMap<State> = {
    {"Idle"s, State::Idle},
    {"Fidget"s, State::Fidget},
    {"Walk"s, State::Walk},
    {"Scan"s, State::Scan},
    {"Attack"s, State::Attack}};

template <typename T>
T stringToEnum(const std::string& str) {
    return static_cast<T>(myMap<T>.at(str));
}

template <typename T>
const std::string& enumToString(T eToFind) {
    for (const auto [str, e] : myMap<T>) {
        if (e == eToFind) {
            return str;
        }
    }

    // throw std::exception("Some exception occured."s);
    throw std::exception();
}