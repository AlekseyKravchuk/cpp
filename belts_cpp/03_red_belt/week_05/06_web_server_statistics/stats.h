#pragma once

#include <map>
#include <set>
#include <string_view>
#include <vector>

#include "http_request.h"

using namespace std::literals;

class Stats {
   public:
    // если метод протокола HTTP, переданный в метод AddMethod, не поддерживается нашим сервером,
    // то нужно на единицу увеличить счётчик для метода «UNKNOWN» (подробнее см. юнит-тесты в заготовке решения);
    void AddMethod(std::string_view method);

    // если URI, переданный в метод AddUri, не поддерживается нашим сервером, то нужно на единицу увеличить счётчик для URI «unknown».
    void AddUri(std::string_view uri);

    // метод GetMethodStats возвращает словарь, в котором для каждого метода хранится,
    // сколько раз он встретился в качестве аргумента метода AddMethod;
    const std::map<std::string_view, int>& GetMethodStats() const;

    // метод GetUriStats работает аналогично для URI;
    const std::map<std::string_view, int>& GetUriStats() const;

    friend HttpRequest ParseRequest(std::string_view line);

   private:
    std::map<std::string_view, int> _method_stats = {
        {"GET"s, 0},
        {"POST"s, 0},
        {"PUT"s, 0},
        {"DELETE"s, 0},
        {"UNKNOWN"s, 0}};

    std::map<std::string_view, int> _uri_stats = {
        {"/"s, 0},
        {"/order"s, 0},
        {"/product"s, 0},
        {"/basket"s, 0},
        {"/help"s, 0},
        {"/unknown"s, 0}};
};

std::string_view FindMethodFromSupported(std::string_view s_view);

std::string_view FindUriFromSupported(std::string_view s_view);

std::string_view FindProtocolFromSupported(std::string_view s_view);

HttpRequest ParseRequest(std::string_view line);
