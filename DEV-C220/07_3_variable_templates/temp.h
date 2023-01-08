#pragma once

#include <iterator>
#include <ostream>
#include <string>

template <typename T>
std::ostream_iterator<T> superSeparator{std::cout};  // без разделителя

// специализация шаблона
template <>
std::ostream_iterator<int> superSeparator<int> = std::ostream_iterator<int>(std::cout, " ");  // разделитель - пробел

// специализация шаблона
template <>
std::ostream_iterator<double> superSeparator<double> = std::ostream_iterator<double>(std::cout, ":");  // разделитель - двоеточие

// специализация шаблона
template <>
std::ostream_iterator<std::string> superSeparator<std::string> = std::ostream_iterator<std::string>(std::cout, "\\");  // разделитель - backslash