#pragma once
#include <string>

// генеральный шаблон разделителя - в общем случае использовать в качестве разделителя - пробел
template <typename T>
const char* separator = " ";

// в случае "double" использовать в качестве разделителя двоеточие
template <>
const char* separator<double> = " : ";

// в случае "std::string" использовать в качестве разделителя одиночный backslash
template <>
const char* separator<std::string> = " \\ ";