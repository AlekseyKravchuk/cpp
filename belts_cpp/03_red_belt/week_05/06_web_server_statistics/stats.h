#pragma once

#include <map>
#include <string_view>

#include "http_request.h"
using namespace std;

class Stats {
   public:
    // если метод протокола HTTP, переданный в метод AddMethod, не поддерживается нашим сервером,
    // то нужно на единицу увеличить счётчик для метода «UNKNOWN» (подробнее см. юнит-тесты в заготовке решения);
    void AddMethod(string_view method);

    // если URI, переданный в метод AddUri, не поддерживается нашим сервером, то нужно на единицу увеличить счётчик для URI «unknown».
    void AddUri(string_view uri);

    // метод GetMethodStats возвращает словарь, в котором для каждого метода хранится,
    // сколько раз он встретился в качестве аргумента метода AddMethod;
    const map<string_view, int>& GetMethodStats() const;

    // метод GetUriStats работает аналогично для URI;
    const map<string_view, int>& GetUriStats() const;
};

HttpRequest ParseRequest(string_view line);
