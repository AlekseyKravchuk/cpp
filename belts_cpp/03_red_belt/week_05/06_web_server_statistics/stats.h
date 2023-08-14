#pragma once

#include <map>
#include <set>
#include <string_view>
#include <vector>

#include "http_request.h"

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
        {"GET", 0},
        {"POST", 0},
        {"PUT", 0},
        {"DELETE", 0},
        {"UNKNOWN", 0}};

    std::map<std::string_view, int> _uri_stats = {
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0}};
};

HttpRequest ParseRequest(std::string_view line);

// Ключевое слово inline позволяет определить статические члены SUPPORTED_METHODS, SUPPORTED_URIs, SUPPORTED_PROTOCOLS, UNSUPPORTED_STORAG
// здесь, в .h-файле. Без "inline" нам бы пришлось объявить их здесь (с помощью ключевого слова "extern"), а определеление вынести в stats.cpp
inline static const std::set<std::string_view> SUPPORTED_METHODS = {"GET", "POST", "PUT", "DELETE"};
inline static const std::set<std::string_view> SUPPORTED_URIs = {"/", "/order", "/product", "/basket", "/help"};
inline static const std::set<std::string_view> SUPPORTED_PROTOCOLS = {"HTTP/1.1"};

// глобальное хранилище для хранения названий неподдерживаемых методов, URI и протоколов
inline static std::set<std::string> UNSUPPORTED_STORAGE{};
