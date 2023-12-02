#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <tuple>


#include "test_runner.h"

void TestEmpty();

// функция "TestAll()" должна вызываться в самом начале функции "main"
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll();
