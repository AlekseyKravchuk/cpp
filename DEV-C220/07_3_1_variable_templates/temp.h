#pragma once

// объявление ШАБЛОННОЙ ПЕРЕМЕННОЙ: шаблонная переменная в данном случае РОВНО ТАКОГО же типа, как и параметр шаблон
// template <typename T> T var;

template <typename T> int var;

// специализации шаблонных переменных
template <> int var<char> = 1;

template <> int var<double> = 2;
